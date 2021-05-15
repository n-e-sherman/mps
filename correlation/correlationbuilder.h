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
		auto bra_args = _bra_args(args);

		cout << "building correlation -- key: " << key << endl;
		repo = repoBuilder->build(args, key);
		auto load = args->getBool("load"); 

		auto correlation = repo->load(Correlation::getHash(args), new Correlation(args, evolverBuilder->build(bra_args, key), evolverBuilder->build(args, key), measurementBuilder->build(args, key)));
		if((correlation != nullptr) && load) return correlation;
		correlation = new Correlation(args, evolverBuilder->build(bra_args, key), evolverBuilder->build(args, key), measurementBuilder->build(args, key), stateBuilder->build(args, key), operatorBuilder->build(args, key));
		repo->save(Correlation::getHash(args), correlation);
		return correlation;
	}

private:
	// virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
	Args* _bra_args(Args* args)
	{	
		auto bra_args = new Args(*args);
		if(bra_args->defined("braEvovler"))
			bra_args->add("Evolver",args->getString("braEvolver"));
		else
		{
			if(!(bra_args->getBool("thermal")))
				bra_args->add("Evolver","Trivial");
		}
		return bra_args;
		// return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
	}
};
#endif