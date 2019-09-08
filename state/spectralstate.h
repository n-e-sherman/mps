#ifndef __SPECTRALSTATE_H_
#define __SPECTRALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"

using namespace itensor;
using namespace std;

class SpectralState : public State
{
protected:

	void calcSpectralState()
	{
		/* Imaginary time evolved psi. */
	}
	void calcPsi()
	{
		/* Make maximally entangled state */
	}

public:

	SpectralStateCalculator(InputGroup* i) : State(i) 
	{
		calcSpectralState();
	}
	~SpectralStateCalculator() {}	
	static string getHash() // This Hash needs some love.
	{
		return "SpectralState_" + m->getHash(); // Model is unnecesary, need some type of params, maybe don't even save?
	}
	virtual string getHash() {return SpectralState::getHash(model); }
};
#endif