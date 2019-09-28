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
		/* Implement cooling */
	}
	void calcPsii() // Makes an infinite temperature state via purification.
	{
		auto sites = model->getSites();
		state = MPS(sites);
		auto N = sites.length();
    	for(int n = 1; n <= N; n += 2)
        {
        	cout << n << "," << N << endl;
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