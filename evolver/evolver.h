#ifndef __EVOLVER_H_
#define __EVOLVER_H_

#include "itensor/all.h"
#include "evolver/evolverbuilder.h"

using namespace itensor;
using namespace std;

class Evolver
{
protected:
	/* Inputs */
	Args* args;
	SiteSet sites;
	Model* model;

	/* Outputs */

public:
	Evolver(){}
	Evolver(Args* a, Model* m) : args(a), model(m) {}
	virtual void evolve(MPS & psi) = 0;
	virtual void setup(BondGate::Type type, Real tau, string op = "H") = 0;
};
#endif