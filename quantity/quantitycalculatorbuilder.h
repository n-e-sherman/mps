#ifndef __QUANTITYCALCULATORBUILDER_H_
#define __QUANTITYCALCULATORBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/builder.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/statecalculator.h"
// #include "state/thermalstatecalculator.h"
// #include "state/statecalculatorbuilder.h"

using namespace itensor;

class QuantityCalculator : public Calculator
{
protected:
	InputGroup* input;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	StateCalculator* stateCalculator;
	MPS* state;
public:
	QuantityCalculator(ModelBuilder* mb, StateCalculator* sc, RepositoryBuilder* rb) : Calculator() {modelBuilder = mb; stateCalculator = sc; repoBuilder = rb; }
	~QuantityCalculator() {}	
	Calculator* build(InputGroup* i)
	{
		input = i;
		auto thermal = input->getYesNo("thermal",0);
		if(thermal) model = modelBuilder->build(input,ModelBuilder::thermal);
		else model = modelBuilder->build(input);
		// sites = model->getSites();
		repo = repoBuilder->build();
		Calculator* c;
		/* check for the quantity type. spectral, */
		return c;
	}
};
#endif