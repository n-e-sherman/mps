#ifndef __EXPMPO_H_
#define __EXPMPO_H_

#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class ExpMPO : public Evolver
{
protected:

	MPO expH;

public:
	ExpMPO(){}
	ExpMPO(Args* a, Model* m) : Evolver(a,m) {}

	virtual void evolve(State& s)
	{
		s.setState(noPrime(applyMPO(model->getO(),s.getState())));
	}
	
	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		auto _tau = Cplx(tau);
		if(type == BondGate::tReal)
		{
			_tau *= Complex_i;
		}
		if(op == "H"){ model->calcExpH(_tau); expH = model->getO(); }
		else 		 {model->calcExpL(_tau); expH = model->getO(); }
	}
};
#endif
