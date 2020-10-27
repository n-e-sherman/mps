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
	/* Inputs */
	Args* args;
	CorrelationBuilder* correlationBuilder;
	Correlation* correlation;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

	/* Outputs */
	vector<Real> res;
	vector<string> labels;
	vector<vector<StringReal>> results;



public:

	CorrelationService(CorrelationBuilder* cb, RepositoryBuilder* rb) : correlationBuilder(cb), repoBuilder(rb) {} 

	void calculate(Args* a)
	{
		args = a;
		repo = repoBuilder->build(args);
		correlation = correlationBuilder->build(args);
		string type = (args->getBool("momentum") ? "p" : "x");
		repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model"),labels,results);
		auto time_max = args->getReal("time");
		while(correlation->getTime() < time_max)
		{
			cout << "time = " << correlation->getTime() << endl;
			correlation->calculate();
			repo->save(Correlation::getHash(args),correlation);
			auto [labels,results] = correlation->getResults();
			repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model"),labels,results);
		}
		/* 
		 - Loop over time steps needed
		 - For each time step
		 	. calculate correlation
		 	. save correlation
		 	. Save results
		 */

	}
};

#endif
