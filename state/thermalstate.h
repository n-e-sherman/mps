#ifndef __THERMALSTATE_H_
#define __THERMALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "state/state.h"
#include <cmath>

using namespace itensor;
using namespace std;

class ThermalState : public State
{
protected:

	Args* args;
	Sites* sites;
	Real beta;
	Evolver* evolver;

public:

	ThermalState(Args* a) : args(a) {}
	ThermalState(Args* a, Evolver* e) : args(a), evolver(e)
	{
		sites = evolver->getSites();
		beta = args->getReal("beta");
		calcInitialState();
		if(beta > 0) coolState();
		state.orthogonalize();
        state.normalize();
	}

private:
	
	void calcInitialState()
	{
		state = MPS(sites->getSites());
		auto N = sites->getSites().length();
    	for(int n = 1; n <= N; n += 2)
        {
	        auto s1 = sites->getSites()(n);
	        auto s2 = sites->getSites()(n+1);
	        auto wf = ITensor(s1,s2);
	        wf.set(s1(1),s2(2), ISqrt2);
	        wf.set(s1(2),s2(1), -ISqrt2);
	        ITensor D;
	        state.ref(n) = ITensor(s1);
	        state.ref(n+1) = ITensor(s2);
	        svd(wf,state.ref(n),D,state.ref(n+1));
	        state.ref(n) *= D;
        }
        
        // Define bond indices.
    	auto links = std::vector<Index>(N+1);
		for(auto n : range(N+1))
	    {
		    auto tags = format("Link,l=%d",n);
		    if(hasQNs(state))
		    	links.at(n) = Index(QN(),1,tags);
		    else
		    	links.at(n) = Index(1,tags);
	    }
		for(int n = 2; n <= N-1; n += 2)
	    {
		    auto l = links.at(n);
		    state.ref(n) = state(n)*setElt(l(1));
		    state.ref(n+1) = state(n+1)*setElt(itensor::dag(l)(1));
	    }
        
	}

	void coolState()
	{
		auto tau = args->getReal("beta-tau");
		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	if(fabs(nt*tau-ttotal) > eps) { nt += 1; tau = ttotal/Real(nt); }
    	evolver->setup(BondGate::tImag, tau);
    	for(int tt=1; tt <= nt; ++tt) {evolver->evolve(*this); }
	}
};
#endif