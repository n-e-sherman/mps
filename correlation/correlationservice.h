#ifndef __CORRELATIONSERVICE_H_
#define __CORRELATIONSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "correlation/correlationbuilder.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace itensor;
using namespace std;

class CorrelationService
{
private:

	CorrelationBuilder* correlationBuilder;
	Correlation* correlation;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:

	CorrelationService(CorrelationBuilder* cb, RepositoryBuilder* rb) : correlationBuilder(cb), repoBuilder(rb) {} 

	void calculate(Args* args_in)
	{
		auto key =  "correlationService";
		auto args = build_args(args_in, key);

		repo = repoBuilder->build(args, key);
		auto save = args->getBool("save");

		correlation = correlationBuilder->build(args, key);
		repo->save(Correlation::getHash(args),correlation,save);
		
		auto time_max = args->getReal("time");
		string type = (args->getBool("momentum") ? "p" : "x");
		if(!(args->getBool("realTime"))) type = "i" + type;
		while(abs(correlation->getTime()) < abs(time_max))
		{
			cout << "time = " << correlation->getTime() << " / " << time_max << endl;
			correlation->calculate();
			repo->save(Correlation::getHash(args),correlation,save);
			auto [labels,results] = correlation->getResults();
			repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model")+"/"+args->getString("Lattice"),labels,results); //<--- Update
		}
		auto [labels,results] = correlation->getResults();
		repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model")+"/"+args->getString("Lattice"),labels,results,true); //<--- Update
	}
};

#endif
