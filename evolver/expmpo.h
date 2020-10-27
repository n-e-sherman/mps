#ifndef __EXPMPO_H_
#define __EXPMPO_H_

#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class ExpMPO : public Evolver
{
protected:
	/* Inputs */
	Args* args;
	SiteSet sites;
	Model* model;

	/* Helpers */
	MPO expH;

	/* Outputs */


public:
	ExpMPO(){}
	ExpMPO(Args* a, Model* m) : args(a), model(m) {}
	void evolve(MPS & psi)
	{
		psi = noPrime(applyMPO(expH,psi));
	}
	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		auto _tau = Cplx(tau);
		if(type == BondGate::tReal)
		{
			_tau *= Complex_i;
		}
		if(op == "H"){ expH = model->getExpH(_tau,true); }
		else 		 {expH = model->getExpL(_tau,true);}
		/**********************************************
		 * This may make things more accurace somehow *
		 **********************************************/

		// auto taua = tau/2.*(1.+1._i);
  		// auto taub = tau/2.*(1.-1._i);
  		// auto expHa = model->getExpH(taua);
  		// auto expHb = model->getExpH(taub);
  		// for(int tt = 1; tt <= nt; ++tt)
  		// {
  		// 	state = noPrime(applyMPO(expHa,state));
  		// 	state = noPrime(applyMPO(expHb,state));
  		// }
	}
};
#endif
