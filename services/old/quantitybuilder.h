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
#include "quantity/quantity.h"
// #include "state/thermalstatecalculator.h"
// #include "state/statecalculatorbuilder.h"

using namespace itensor;

class QuantityService : public Service
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	StateBuilder* stateBuilder;
	State* state;
public:
	QuantityService(ModelBuilder* mb, StateBuilder* sb, RepositoryBuilder* rb)
	{
		modelBuilder = mb; 
		stateBuilder = sb; 
		repoBuilder = rb; 
	}
	~QuantityService() {}	
	Quantity* Calculate(Args* a)
	{
		args = a;
		repo = repoBuilder->build();
		Quantity* q;
		auto thermal = args->getYesNo("thermal",0);
		state = stateBuilder->build(args,StateBuilder::spectral);
		if(thermal) {model = modelBuilder->build(args,ModelBuilder::thermal); }
		else model = modelBuilder->build(input);
		auto spectral = args->getYesNo("spectral",0);
		if(spectral)
		{

		}
		/* check for the quantity type. spectral, */
		return c;
	}
};
#endif