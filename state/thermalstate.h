#ifndef __THERMALSTATE_H_
#define __THERMALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "evolver/evolverbuilder.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include <cmath>

using namespace itensor;
using namespace std;

class ThermalState : public State
{
protected:

	Real beta;
	Evolver* evolver;

public:

	ThermalState(){}
	ThermalState(Args* a, Model* m, Evolver* e) : State(a,m), evolver(e)
	{
		beta = args->getReal("beta");
		calcInitialState();
		if(beta > 0) coolState();
		state.orthogonalize();
        state.normalize();
	}
	~ThermalState() {}	

private:
	
	void calcInitialState() // Makes an infinite temperature state via purification.
	{
		auto sites = model->getSites();
		state = MPS(sites);
		auto N = sites.length();
    	for(int n = 1; n <= N; n += 2)
        {
	        auto s1 = sites(n);
	        auto s2 = sites(n+1);
	        auto wf = ITensor(s1,s2);
	        wf.set(s1(1),s2(2), ISqrt2);
	        wf.set(s1(2),s2(1), -ISqrt2);
	        ITensor D;
	        state.Aref(n) = ITensor(s1);
	        state.Aref(n+1) = ITensor(s2);
	        svd(wf,state.Aref(n),D,state.Aref(n+1));
	        state.Aref(n) *= D;
        }
        
	}

	void coolState()
	{
		auto tau = args->getReal("tau");
		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	if(fabs(nt*tau-ttotal) > eps) { nt += 1; tau = ttotal/Real(nt); } // Adjust tau to evenly divide beta/2
    	evolver->setup(BondGate::tImag, tau);
    	for(int tt=1; tt <= nt; ++tt) {evolver->evolve(state); } // cool state down.
	}
};
#endif