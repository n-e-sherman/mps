#ifndef __STATE_H_
#define __STATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"

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
	State(Model* m, Args* a) 
	{
		model = m;
		args = a;
		E0 = 0;
	}
	MPS& applyMPO(const MPO& H){ return applyMPO(H,state,args); }
	MPS& getState() { return state; }
	Real getE0() {return E0; }
	static string getHash(Model* m)
	{
		return m->getHash(); // Maybe add some specifications for how you get GS?
	}
	virtual string getHash() {return State::getHash(model); }
};
#endif