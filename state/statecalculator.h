#ifndef __STATECALCULATOR_H_
#define __STATECALCULATOR_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/groundstate.h"
#include "state/thermalstate.h"
#include "state/spectralstate.h"
#include "state/state.h"

using namespace itensor;
using namespace std;

class StateCalculator : public Calculator
{
protected:
	InputGroup* input;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	SiteSet sites;
public:
	enum stateType
	{
		ground = 0,
		thermal = 1,
		spectral = 2
	};

	StateCalculator(ModelBuilder* mb, RepositoryBuilder* rb) : Calculator() {modelBuilder = mb; repoBuilder = rb; }
	~StateCalculator() {}	
	State::stateInfo build(InputGroup* i, stateType sType = ground)
	{
		// auto sType = input.getString()
		input = i;
		State* s;
		if(sType == ground) 
		{
			model = modelBuilder->build(input);
			repo = repoBuilder->build();
			s = new GroundState(model,input);
		}
		else if(sType == thermal)
		{
			model = modelBuilder->build(input);
			repo = repoBuilder->build();
			s = new ThermalState(model, repo);
		}
		else if(sType == spectral)
		{
			/* Think about the logic, since you need a different state */
		}
		// else if(sType == thermal) c = new ThermalStateCalculator(modelBuilder,repoBuilder);
		// else if(sType == spectral) c = new SpectralStateCalculator(modelBuilder,repoBuilder);
		return s->getState();

	}
};
#endif