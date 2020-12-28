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

	void calculate(Args* args)
	{
		repo = repoBuilder->build(args);
		correlation = correlationBuilder->build(args);
		string type = (args->getBool("momentum") ? "p" : "x");
		auto time_max = args->getReal("time");
		while(correlation->getTime() < time_max)
		{
			cout << "time = " << correlation->getTime() << endl;
			correlation->calculate();
			repo->save(Correlation::getHash(args),correlation);
			auto [labels,results] = correlation->getResults();
			repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model"),labels,results); //<--- Update
		}
		auto [labels,results] = correlation->getResults();
		repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model"),labels,results); //<--- Update
	}
};

#endif
