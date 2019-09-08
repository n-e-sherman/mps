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
public:
	struct stateInfo
	{
		MPS* state;
		Real value;
	};
protected:
	InputGroup* input;
	MPS state;
	SiteSet sites;

public:
	State(InputGroup* i) 
	{
		input = i;
	}

	virtual stateInfo getState() {return stateInfo{&state, -1}; }
};
#endif