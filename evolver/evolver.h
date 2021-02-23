#ifndef __EVOLVER_H_
#define __EVOLVER_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class Evolver
{
protected:

	Args* args;
	Model* model;

public:
	
	Evolver(){}
	Evolver(Args* a, Model* m) : args(a), model(m) {}
	virtual void evolve(State& s) = 0;
	virtual void setup(BondGate::Type type, Real tau, string op = "H") = 0;
	map<string, Real> getParams() {return model->getParams(); }
	Sites* getSites(){ return model->getSites(); }
	virtual void read(istream& is) 
	{
		model->read(is);
	}
	virtual void write(ostream& os) const 
	{
		model->write(os);
	}

};
#endif