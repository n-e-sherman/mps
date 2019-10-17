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
        cout << "Norm of thermalstate= " << innerC(psi0,psi0) << endl;
        cout << "Norm of spectralstate= " << innerC(state,state) << endl;
        cout << "Innerproduct= " << innerC(state,psi0) << endl;
        prepare(state,psi0,is);
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