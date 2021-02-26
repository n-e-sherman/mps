#ifndef __TDVP_H_
#define __TDVP_H_

#include "evolver/evolver.h"
#include "addons/itensor-tdvp.h"

using namespace itensor;
using namespace std;

class TDVP : public Evolver
{
protected:

	Sweeps sweeps;
	Cplx dt;

public:

	TDVP(){}
	TDVP(Args* a, Model* m) : Evolver(a,m) {}

	virtual void evolve(State& s)
	{
		tdvp(s.getState(),model->getO(),-dt,sweeps,*args);
	}

	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		dt = Cplx(tau);
		if(type == BondGate::tReal)
		{
			dt *= Complex_i;
		}
		if(op == "H"){ model->calcH(); }
		else         { model->calcL(); }

		/* Include other sweep params in the future. */
		sweeps = Sweeps(1);
	    sweeps.maxdim() = args->getInt("MaxDim");
	    sweeps.niter() = args->getInt("MaxIter");
	    
	}

	/* Don't need read & write */

};

class TDVPswap : public TDVP
{
protected:

public:

	TDVPswap(){}
	TDVPswap(Args* a, Model* m) : TDVP(a,m) {}
	virtual void evolve(State& s)
	{
		if((s.maxLinkDim() >= args->getInt("MaxDim")))
			tdvp(s.getState(),model->getO(),-dt,sweeps,(*args) += {"NumCenter",1});	
		else
			tdvp(s.getState(),model->getO(),-dt,sweeps,(*args) += {"NumCenter",2});	
	}

};
#endif

