#ifndef __STATE_H_
#define __STATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/statebuilder.h"

using namespace itensor;
using namespace std;

class State
{
protected:
	/* Inputs */
	Args* args;
	SiteSet sites;
	Model* model;

	/* Outputs */
	Real E0;
	MPS state;

public:
	State(){}
	State(Args* a, Model* m) : args(a), model(m), E0(0) {}
	MPS& getState() { return state; }
	Real getE0() {return E0; }
	static string getHash(Args* args)
	{	
		Real s = 0;
		if(args->getBool("momentum")) s = args->getReal("qFactor");
		string sThermal = "";
		if(args->getBool("thermal")) sThermal = to_string(args->getReal("beta"));
		return Model::getHash(args) + "_" + to_string(args->getInt("MaxDim")) + "_" + to_string(s) + "_" + to_string(args->getBool("thermal"))  + "_" + sThermal; // Maybe add some specifications for how you get GS?
	}
	virtual void load(ifstream & f)
	{
		read(f,E0);
		read(f,state);
	}
	virtual void save(ofstream & f)
	{
		write(f,E0);
		write(f,state);
	}
};
#endif