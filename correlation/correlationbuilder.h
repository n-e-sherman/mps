#ifndef __CORRELATIONBUILDER_H_
#define __CORRELATIONBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "correlation/correlation.h"
#include "correlation/correlationx.h"
#include "correlation/correlationp.h"
#include "model/modelbuilder.h"
#include "repository/repository.h"
#include "state/statebuilder.h"
#include "evolver/evolverbuilder.h"
#include "measurement/measurementbuilder.h"


class CorrelationBuilder
{
protected:
	StateBuilder* stateBuilder;
	EvolverBuilder* evolverBuilder;
	ModelBuilder* modelBuilder;
	RepositoryBuilder* repoBuilder;
	MeasurementBuilder* measurementBuilder;
	OperatorBuilder* operatorBuilder;
	Repository* repo;

public:
	CorrelationBuilder(StateBuilder* sb, EvolverBuilder* eb, ModelBuilder* mb, RepositoryBuilder* rb, MeasurementBuilder* msb, OperatorBuilder* ob) : 
					   stateBuilder(sb), evolverBuilder(eb), modelBuilder(mb), repoBuilder(rb), measurementBuilder(msb), operatorBuilder(ob) {}
	~CorrelationBuilder(){}
	Correlation* build(Args* args)
	{
		repo = repoBuilder->build(args);
		auto momentum = args->getBool("momentum");
		auto thermal = args->getBool("thermal");
		if(momentum) 
		{
			auto correlation = repo->load(Correlation::getHash(args), 
				new Correlationp(args, 
							     evolverBuilder->build(args), 
							     modelBuilder->build(args)));
			if(correlation != nullptr) return correlation;
			correlation = new Correlationp(args, 
										   stateBuilder->build(args), 
										   evolverBuilder->build(args), 
										   modelBuilder->build(args), 
										   operatorBuilder->build(args));
			repo->save(Correlation::getHash(args), correlation);
			return correlation;
		}
		else
		{
			auto correlation = repo->load(Correlation::getHash(args), 
				new Correlationx(args, 
							     evolverBuilder->build(args), 
							     modelBuilder->build(args), 
							     measurementBuilder->build(args)));
			if(correlation != nullptr) return correlation;
			correlation = new Correlationx(args, 
										   stateBuilder->build(args), 
										   evolverBuilder->build(args), 
										   modelBuilder->build(args), 
										   measurementBuilder->build(args));
			repo->save(Correlation::getHash(args), correlation); 
			return correlation;
		}
	}
};
#endif