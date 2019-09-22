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
		auto c = args->getInt("c");
		auto N = args->getInt("N");
		auto psi0 = in_state->getState();
		auto is = siteInds(psi0);
        op.position(c);
        psi0.position(c);
        state = applyMPO(op,psi0);
        prepare(state,psi0,c,is);
	}
	void prepare(MPS &a, MPS &b, int c, IndexSet is)
	{
	    a.replaceSiteInds(is);
	    b.replaceSiteInds(is);
	    a.position(c);
	    b.position(c);
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