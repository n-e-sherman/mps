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

class StateBuilder : public Builder
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	LatticeBuilder* latticeBuilder;
	Lattice* lattice;
	SiteSet sites;
public:
	enum stateType
	{
		ground = 0,
		thermal = 1,
		spectral = 2
	};

	StateBuilder(ModelBuilder* mb, RepositoryBuilder* rb, LatticeBuilder* lb) : Builder() {modelBuilder = mb; repoBuilder = rb; latticeBuilder = lb; }
	~StateBuilder() {}	
	State* build(Args* a, stateType sType = ground)
	{
		args = a;
		model = modelBuilder->build();
		sites = model->getSites();
		return build(sType);
	}

private:
	State* build(stateType sType = ground)
	{
		State* s;
		if(sType == ground) {s = new GroundState(args, model, buildInitialState()); }
		else if(sType == thermal) {s = new ThermalState(args, model); }
		else if(sType == spectral)
		{
			lattice = latticeBuilder->build(args);
			auto thermal = args.getYesNo("thermal",0);
			if(thermal) {s = new SpectralState(args, model, build(thermal)); }
			else{s = new SpectralState(model, args, build(ground), buildOperator()); }
			/* May want to adjust this if you have more than two options. */
		}
		return s;
	}




	MPS buildInitialState()
	{
		auto init = args->getString("initial","AF");
		if (init == "AF")
		{
			auto N = args->getInt("N",100);
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
		auto mom = args->getYesNo("momentum",1);
		if(mom)
		{
			auto qfactor = args->getReal("qfactor",1.0);
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