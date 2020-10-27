#ifndef __GROUNDSTATE_H_
#define __GROUNDSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/state.h"
#include <string>

using namespace itensor;
using namespace std;

class GroundState : public State
{
public:

	GroundState() : State() {}
	GroundState(Args* a,Model* m) : State(a,m)
	{
		buildInitialState();
		calcGroundState();
	}
	~GroundState() {}

private:

	void calcGroundState()
	{
		auto nsweeps = args->getInt("nsweeps",5);
        auto sweeps = getSweeps(); // how?
        auto [energy,psiout] = dmrg(model->getH(),state,sweeps,"Quiet");
        E0=energy;
        state=psiout;
	}

	void buildInitialState()
	{
		auto init = args->getString("initial");
		if (init == "AF")
		{
			auto N = args->getInt("N");
			auto _state = InitState(sites);
	        for(auto i : range1(N))
	            {
	            if(i%2 == 1) _state.set(i,"Up");
	            else         _state.set(i,"Dn");
	            }
	        state = MPS(_state);
    	}
    	else // (init == "F")
    	{
    		auto N = args->getInt("N");
			auto _state = InitState(sites);
	        for(auto i : range1(N))
	            {
		            _state.set(i,"Up");
	            }
	        state = MPS(_state);	
    	}
    	/* Include other initial states with an else if.*/
	}

	Sweeps getSweeps()
	{
		Sweeps sweeps(args->getInt("nSweeps"));
		auto maxdim = stringToVectorI(args->getString("sweeps_maxdim"));
		for(auto i : range(maxdim.size())) sweeps.setmaxdim(i+1,maxdim[i]);
		auto mindim = stringToVector(args->getString("sweeps_mindim"));
		for(auto i : range(mindim.size())) sweeps.setmindim(i+1,mindim[i]);
		auto cutoff = stringToVector(args->getString("sweeps_cutoff"));
		for(auto i : range(cutoff.size())) sweeps.setcutoff(i+1,cutoff[i]);
		auto niter = stringToVector(args->getString("sweeps_niter"));
		for(auto i : range(niter.size())) sweeps.setniter(i+1,niter[i]);
		auto noise = stringToVector(args->getString("sweeps_noise"));
		for(auto i : range(noise.size())) sweeps.setnoise(i+1,noise[i]);
		return sweeps;
	}
};
#endif