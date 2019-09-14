#ifndef __GROUNDSTATE_H_
#define __GROUNDSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
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
        auto table = InputGroup(*input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table); // how?
        auto sweeps = Cache::getIstance()->get("sweeps");
        auto [energy,psiout] = dmrg(*(model->getH()),state,sweeps,"Quiet");
        E0=energy;
        state=psiout;
	}

public:

	GroundState(Args* a,Model* m, MPS& in) : State(a,m)
	{
		state = in;
		calcGroundState();
	}
	~GroundState() {}

	Sweeps getSweeps()
	{
		auto maxdim = stringToVector(args->getString("sweeps_maxdim"));
		auto mindim = stringToVector(args->getString("sweeps_mindim"));
		auto cutoff = stringToVector(args->getString("sweeps_cutoff"));
		auto niter = stringToVector(args->getString("sweeps_niter"));
		auto noise = stringToVector(args->getString("sweeps_noise"));

	}
};
#endif