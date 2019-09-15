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
protected:

	void calcGroundState()
	{
		auto nsweeps = args->getInt("nsweeps",5);
        auto sweeps = getSweeps(); // how?
        auto [energy,psiout] = dmrg(model->getH(),state,sweeps,"Quiet");
        E0=energy;
        state=psiout;
	}

public:

	GroundState() : State() {}
	GroundState(Args* a,Model* m, const MPS& in) : State(a,m)
	{
		state = in;
		calcGroundState();
	}
	~GroundState() {}

	Sweeps getSweeps()
	{
		Sweeps sweeps;
		sweeps.nsweep(args->getInt("nSweeps"));
		auto maxdim = stringToVector(args->getString("sweeps_maxdim"));
		for(auto i : range(maxdim.size())) sweeps.setmaxdim(i,maxdim[i]);
		auto mindim = stringToVector(args->getString("sweeps_mindim"));
		for(auto i : range(mindim.size())) sweeps.setmindim(i,mindim[i]);
		auto cutoff = stringToVector(args->getString("sweeps_cutoff"));
		for(auto i : range(cutoff.size())) sweeps.setcutoff(i,cutoff[i]);
		auto niter = stringToVector(args->getString("sweeps_niter"));
		for(auto i : range(niter.size())) sweeps.setniter(i,niter[i]);
		auto noise = stringToVector(args->getString("sweeps_noise"));
		for(auto i : range(noise.size())) sweeps.setnoise(i,noise[i]);
		return sweeps;
	}
};
#endif