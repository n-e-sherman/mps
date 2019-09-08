#ifndef __THERMALSTATE_H_
#define __THERMALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"

using namespace itensor;
using namespace std;

class ThermalState : public State
{
protected:
	Model* model;

	void calcThermalState()
	{
		/* Imaginary time evolved psi. */
	}
	void calcPsi()
	{
		/* Make maximally entangled state */
	}

public:

	ThermalStateCalculator(Model* m, InputGroup* i) : State(i) 
	{
		model = m;
		calcThermalState();
	}
	~ThermalStateCalculator() {}	
	// MPS const * const calculate(InputGroup* i)
	// {
	// 	input = i;
	// 	model = modelBuilder->build(input);
	// 	sites = model->getSites();
	// 	repo = repoBuilder->build();
	// 	calcThermalState();
	// 	return &psi;
	// }
	static string getHash(Model* m)
	{
		return "ThermalState_" + m->getHash(); // Going to want a value for beta too.
	}
	virtual string getHash() {return ThermalState::getHash(model); }
};
#endif