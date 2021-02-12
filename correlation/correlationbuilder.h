#ifndef __CORRELATIONBUILDER_H_
#define __CORRELATIONBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "correlation/correlation.h"
#include "evolver/evolverbuilder.h"
#include "measurement/measurementbuilder.h"
#include "state/statebuilder.h"
#include "operator/operatorbuilder.h"
#include "repository/repository.h"

using namespace itensor;
using namespace std;

class CorrelationBuilder
{
protected:

	EvolverBuilder* evolverBuilder;
	MeasurementBuilder* measurementBuilder;
	StateBuilder* stateBuilder;
	OperatorBuilder* operatorBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:
	
	CorrelationBuilder(EvolverBuilder* eb, MeasurementBuilder* msb, StateBuilder* sb, OperatorBuilder* ob, RepositoryBuilder* rb) : 
					   evolverBuilder(eb), measurementBuilder(msb), stateBuilder(sb), operatorBuilder(ob), repoBuilder(rb) {}
	Correlation* build(Args* args)
	{
		cout << "building correlation" << endl;
		repo = repoBuilder->build(args);
		auto momentum = args->getBool("momentum");
		auto thermal = args->getBool("thermal");

		auto correlation = repo->load(Correlation::getHash(args), new Correlation(args, evolverBuilder->build(args), measurementBuilder->build(args)));
		if(correlation != nullptr) return correlation;
		correlation = new Correlation(args, evolverBuilder->build(args), measurementBuilder->build(args), stateBuilder->build(args), operatorBuilder->build(args));
		repo->save(Correlation::getHash(args), correlation);
		return correlation;
	}
};
#endif