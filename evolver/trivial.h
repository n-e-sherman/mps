#ifndef __TRIVIAL_H_
#define __TRIVIAL_H_

#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class Trivial : public Evolver
{
protected:

	Sweeps sweeps;
	Cplx dt;
	bool normalize = false;

public:

	Trivial(){}
	Trivial(Args* a) : Evolver(a) {}

	virtual void evolve(State& s)
	{
		s.scale(exp(-dt * (s.getE0())));
	}

	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		dt = Cplx(tau);
		if(type == BondGate::tReal)
			dt *= Complex_i;
		// else
		// 	normalize = true;
	}

	virtual void read(istream& is) {}
	virtual void write(ostream& os) const {}

};

#endif

