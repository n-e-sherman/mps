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
		s.setState(noPrime(applyMPO(model->getO(),s.getState(),*args)));
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

/* From Other code ideas */
// auto expH1 = toExpH(ampo,(1-1_i)/2*t0);
//     auto expH2 = toExpH(ampo,(1+1_i)/2*t0);
//     printfln("Maximum bond dimension of expH1 is %d",maxLinkDim(expH1));
//     auto args = Args("Method=","DensityMatrix","Cutoff=",1E-12,"MaxDim=",2000);
//     for(int n = 1; n <= nsw*std::real(t/t0); ++n)
//         {
//         psi2 = applyMPO(expH1,psi2,args);
//         psi2.noPrime();
//         psi2 = applyMPO(expH2,psi2,args);
//         psi2.noPrime().normalize();
//         if(n%int(std::real(t/t0)) == 0)
//             {
//             printfln("\nMaximum bond dimension at time %.1f is %d ", n*t0, maxLinkDim(psi2));
//             printfln("Energy using overlap at time %.1f is %.10f", n*t0, real(innerC(psi2,H,psi2)) );
//             }
//         }
