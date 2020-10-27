#ifndef __STATEBUILDER_H_
#define __STATEBUILDER_H_

/* This needs some serious love, deal with this later */

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
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	SiteSet sites;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	OperatorBuilder* operatorBuilder;
public:
	enum stateType
	{
		ground = 0,
		thermal = 1,
		spectral = 2
	};

	StateBuilder() {}
	StateBuilder(ModelBuilder* mb, LatticeBuilder* lb, RepositoryBuilder* rb, OperatorBuilder* ob) {modelBuilder = mb; repoBuilder = rb; latticeBuilder = lb; operatorBuilder = ob;}
	~StateBuilder() {}	
	State* build(Args* a, stateType sType = ground)
	{
		cout << "building state" << endl;
		args = a;
		repo = repoBuilder->build(args);
		model = modelBuilder->build(args);
		sites = model->getSites();
		return build(sType);
	}

private:
	State* build(stateType sType = ground)
	{
		auto t = args->getBool("thermal");
		if(sType == ground) 
		{
			auto state = repo->load(State::getHash(args,sType), new GroundState());
			if(state != nullptr) return state;
			state = new GroundState(args, model, buildInitialState()); 
			repo->save(State::getHash(args,sType), state);
			return state;
		}
		else if(sType == thermal) 
		{
			auto state = repo->load(State::getHash(args,sType), new ThermalState());
			if(state != nullptr) return state;
			state = new ThermalState(args, model); 
			repo->save(State::getHash(args,sType), state);
			return state;
		}

		else if(sType == spectral)
		{
			auto op = args->getString("Operator");
			args->add("Operator","Momentum");
			auto qFactor = args->getReal("qFactor");
			auto O = operatorBuilder->build(args);
			if(t) 
			{
				auto state = repo->load(State::getHash(args,sType), new SpectralState());
				if(state != nullptr) return state;
				state = new SpectralState(args, model, build(thermal), O->build(qFactor)); 
				repo->save(State::getHash(args,sType), state);
				return state;
			}
			else
			{
				auto state = repo->load(State::getHash(args,sType), new SpectralState());
				if(state != nullptr) return state;
				state = new SpectralState(args, model, build(ground), O->build(qFactor)); 
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
    	if(init == "F")
    	{
    		auto N = args->getInt("N");
			auto _state = InitState(sites);
	        for(auto i : range1(N))
	            {
		            _state.set(i,"Up");
	            }
	        return MPS(_state);	
    	}
    	else
    	{
    		cout << "This should never be called..." << endl;
    		return MPS(sites);
    		/* Fill me in with other options. */ 
    	}
	}

	// MPO buildOperator()
	// {
	// 	auto lattice = latticeBuilder->build(args);
	// 	auto mom = args->defined("qFactor");
	// 	auto N = args->getInt("N");
	// 	if(mom)
	// 	{
	// 		cout << "Building Sq" << endl;
	// 		auto qfactor = args->getReal("qFactor");
	//         auto q = qfactor*M_PI*(Real(N)/Real(N+1));
	//         auto thermal = args->getBool("thermal");
	//         auto coeff = sqrt(2.0/(Real(N+1)));
	//         auto ampo = AutoMPO(sites);
	//         for(auto s : lattice->getSites())
	//         {
	//         	if(s.t == Lattice::physical)
	//         	{
	//         		if(thermal)
	//         		{
	// 	            	ampo += coeff*sin(0.5*(s.s+1)*q),"Sz",s.s;
	//         		}
	// 	            else
	// 	            	ampo += coeff*sin(s.s*q),"Sz",s.s;
	// 	        }
	//         }
	//         auto res = toMPO(ampo);
	//         return res;
	// 	}
	// 	else
	// 	{
	// 		auto I = args->getInt("position",lattice->length()/2);
	// 		auto ampo = AutoMPO(sites);
	// 		ampo += 1.0,"Sz",int(I);
	// 		return toMPO(ampo);
	// 	}

	// }
};
#endif