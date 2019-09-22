#ifndef __SPECTRALSTATE_H_
#define __SPECTRALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "state/state.h"
#include <cmath>

using namespace itensor;
using namespace std;

class SpectralState : public State
{
protected:
	/* Inputs */
	State* in_state;
	MPO op;

	void calcSpectralState()
	{
		auto N = args->getInt("N");
		auto psi0 = in_state->getState();
		auto is = siteInds(psi0);
        op.position(1);
        psi0.position(1);
        state = applyMPO(op,psi0);
        prepare(state,psi0,is);
	}
	void prepare(MPS &a, MPS &b, IndexSet is)
	{
	    a.replaceSiteInds(is);
	    b.replaceSiteInds(is);
	}

public:

	SpectralState() : State(){}
	SpectralState(Args* a,Model* m, State* in, const MPO& o) : State(a,m)
	{
		op = o;
		in_state = in;
		E0 = in_state->getE0();
		calcSpectralState();

	}
	~SpectralState() {}	
};
#endif