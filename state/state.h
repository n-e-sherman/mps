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
	MPS state = MPS();

public:
	State(){}
	State(Args* a, Model* m) 
	{
		model = m;
		args = a;
		E0 = 0;
	}
	MPS& getState() { return state; }
	Real getE0() {return E0; }
	static string getHash(Args* args, int sType = 0)
	{	
		Real s;
		if(args->defined("qFactor")) s = args->getReal("qFactor");
		else s = args->getReal("position");
		int mType = 0;
		if(args->getBool("thermal")) mType = ModelBuilder::thermal;
		return Model::getHash(args,mType) + "_" + to_string(args->getInt("MaxDim")) + "_" + to_string(sType) + "_" + to_string(s); // Maybe add some specifications for how you get GS?
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