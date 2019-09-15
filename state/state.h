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
		string s = "";
		if(args->getBool("momentum")) s = args->getString("qFactor");
		else s = args->getString("position");
		return Model::getHash(args) + "_" + to_string(args->getInt("maxDim")) + "_" + s; // Maybe add some specifications for how you get GS?
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