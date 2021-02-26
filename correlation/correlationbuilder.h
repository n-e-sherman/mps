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
	Correlation* build(Args* args_in, std::string key = "")
	{
		auto base = "correlation";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);

		cout << "building correlation -- key: " << key << endl;
		repo = repoBuilder->build(args, key);
		auto load = args->getBool("load"); 

		auto correlation = repo->load(Correlation::getHash(args), new Correlation(args, evolverBuilder->build(args, key), measurementBuilder->build(args, key)));
		if((correlation != nullptr) && load) return correlation;
		correlation = new Correlation(args, evolverBuilder->build(args, key), measurementBuilder->build(args, key), stateBuilder->build(args, key), operatorBuilder->build(args, key));
		repo->save(Correlation::getHash(args), correlation);
		return correlation;
	}
};
#endif