#ifndef __THERMALSTATE_H_
#define __THERMALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"

using namespace itensor;
using namespace std;

class ThermalState : public State
{
protected:
	Real beta;

	void calcThermalState()
	{
		calcPsii();
		if(beta > 0) coolPsii();
	}
	
	void calcPsii() // Makes an infinite temperature state via purification.
	{
		auto sites = model->getSites();
		state = MPS(sites);
		state = removeQNs(state);
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
        state.orthogonalize();
        state.normalize();
	}

	void coolPsii()
	{
		/* Implement cooling */
		/* TODO: Maybe implement a better cooling here
		 * you are just creating expH from ampo, not ideal
		 * may want some form of Trotter decomp or something
		 * like this in the future.
		 */

		auto ampo = model->getAmpo();
		auto tau = args->getReal("tau");
		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	auto realStep = args->getBool("realStep");
    	auto maxDim = args->getInt("MaxDim");
    	args->add("MaxDim",args->getInt("thermalMaxDim"));
    	auto tSoFar = 0.0;
    	if(fabs(nt*tau-ttotal) > eps)
    	{
    		nt += 1;
    		tau = ttotal/Real(nt);
    	}
		Print(tau);
		Print(nt);
    	if(realStep)
    	{
    		cout << "using real time steps." << endl;
	        auto expH = toExpH(ampo,tau);
	        for(int tt = 1; tt <= nt; ++tt)
	        {
	        	state = noPrime(applyMPO(expH,state,*args));
	        	tSoFar += tau;
	        }
    	}
    	else
    	{
	    	auto taua = tau/2.*(1.+1._i);
	        auto taub = tau/2.*(1.-1._i);
	        auto expHa = toExpH(ampo,taua);
	        auto expHb = toExpH(ampo,taub);
	        for(int tt = 1; tt <= nt; ++tt)
	        {
	        	state = noPrime(applyMPO(expHa,state));
	        	state = noPrime(applyMPO(expHb,state));
	        	tSoFar += tau;
	        }
    	}
    	Print(beta);
    	Print(tSoFar);
        state.orthogonalize();
        state.normalize();
        args->add("MaxDim",maxDim);
	}

public:

	ThermalState(){}
	ThermalState(Args* a, Model* m) : State(a,m)
	{
		beta = args->getReal("beta");
		calcThermalState();
	}
	~ThermalState() {}	
};
#endif