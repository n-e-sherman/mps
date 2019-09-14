#ifndef __STATECALCULATOR_H_
#define __STATECALCULATOR_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"

using namespace itensor;
using namespace std;

class StateBuilder : public Builder
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
public:
	enum stateType
	{
		ground = 0,
		thermal = 1,
		spectral = 2
	};

	StateBuilder(ModelBuilder* mb, LatticeBuilder* lb, RepositoryBuilder* rb,) : Builder() {modelBuilder = mb; repoBuilder = rb; latticeBuilder = lb; }
	~StateBuilder() {}	
	State* build(Args* a, stateType sType = ground)
	{
		args = a;
		repo = repoBuilder->build(args);
		return build(sType);
	}

private:
	State* build(stateType sType = ground)
	{
		auto thermal = args->getBool("thermal");
		if(sType == ground) 
		{
			auto state = repo->load(State::getHash(args,sType), new GroundState());
			if(state != nullptr) return state;
			state = new GroundState(args, modelBuilder->build(), buildInitialState()); 
			repo->save(State::getHash(args,sType), state);
			return state;
		}
		else if(sType == thermal) 
		{
			auto state = repo->load(State::getHash(args,sType), new ThermalState());
			if(state != nullptr) return state;
			state = new ThermalState(args, modelBuilder->build()); 
			repo->save(State::getHash(args,sType), state);
			return state;
		}

		else if(sType == spectral)
		{
			if(thermal) 
			{
				auto state = repo->load(State::getHash(args,sType), new SpectralState());
				if(state != nullptr) return state;
				state = new SpectralState(args, modelBuilder->build(), build(thermal), buildOperator()); 
				repo->save(State::getHash(args,sType), state);
				return state;
			}
			else
			{
				auto state = repo->load(State::getHash(args,sType), new SpectralState());
				if(state != nullptr) return state;
				state = new SpectralState(args, modelBuilder->build(), build(ground), buildOperator()); 
				repo->save(State::getHash(args,sType), state);
				return state;
			}
		}
		else
		{
			/* Implement other states here, may want to use else if. */
			return nullptr;
		}
	}




	MPS buildInitialState()
	{
		auto init = args->getString("initial","AF");
		if (init == "AF")
		{
			auto N = args->getInt("N");
			auto _state = InitState(sites);
	        for(auto i : range1(N))
	            {
	            if(i%2 == 1) _state.set(i,"Up");
	            else         _state.set(i,"Dn");
	            }
	        return MPS(_state);
    	}
    	else
    	{
    		/* Fill me in with other options. */ 
    	}
	}

	MPO buildOperator()
	{
		auto mom = args->getBool("momentum");
		if(mom)
		{
			auto qfactor = args->getReal("qFactor");
	        auto q = qfactor*M_PI*(Real(N)/Real(N+1));
	        auto ampo = AutoMPO(sites);
	        for(auto s : lattice->getSites())
	        {
	        	if(s.t == Lattice::physical)
	            	ampo += sin(s.s*q),"Sz",i;
	        }
	        return toMPO(ampo);
		}
		else
		{
			auto i = args->getInt("i",lattice->length()/2);
			auto ampo = AutoMPO(sites);
			ampo += 1,"Sz",i;
			return toMPO(ampo);
		}

	}
};
#endif