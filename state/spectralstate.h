#ifndef __SPECTRALSTATE_H_
#define __SPECTRALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include <cmath>

using namespace itensor;
using namespace std;

class SpectralState : public State
{
protected:
	/* Inputs */
	State* in_state;
	MPO* op;

	void calcSpectralState()
	{
        Sk.position(c);
        psi0.position(c);
        psii = applyMPO(Sk,psi0);
        prepare(psii,psi0);
        spectralNorm = sqrt(2.0/(N+1));
        psiiNorm = psii.normalize();
	}

public:

	SpectralState(Args* a,Model* m, State* in, MPO& o) : State(a,m)
	{
		op = &o;
		in_state = in;
		E0 = in_state->getE0();
		calcSpectralState();

	}
	~SpectralState() {}	
};
#endif