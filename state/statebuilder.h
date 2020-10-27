#ifndef __STATEBUILDER_H_
#define __STATEBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "operator/operatorbuilder.h"
#include "state/spectralstate.h"
#include "state/groundstate.h"
#include "state/thermalstate.h"
#include "state/state.h"

using namespace itensor;
using namespace std;

class StateBuilder
{
protected:

	ModelBuilder* modelBuilder;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder;
	EvolverBuilder* evolverBuilder;
	Repository* repo;

public:
	StateBuilder(ModelBuilder* mb, LatticeBuilder* lb, RepositoryBuilder* rb, EvolverBuilder* eb) : modelBuilder(mb), latticeBuilder(lb), repoBuilder(rb), evolverBuilder(eb) {}
	~StateBuilder() {}
	State* build(Args* args)
	{
		cout << "building state" << endl;
		repo = repoBuilder->build(args);
		auto thermal = args->getBool("thermal");
		if(thermal)
		{
			auto state = repo->load(State::getHash(args), new ThermalState());
			if(state != nullptr) return state;
			state = new ThermalState(args, modelBuilder->build(args), evolverBuilder->build(args)); 
			repo->save(State::getHash(args), state);
			return state;
		}
		else
		{
			auto state = repo->load(State::getHash(args), new GroundState());
			if(state != nullptr) return state;
			state = new GroundState(args, modelBuilder->build(args)); 
			repo->save(State::getHash(args), state);
			return state;
		}
	}
};
#endif