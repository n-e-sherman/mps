#ifndef __CORRELATIONSSERVICE_H_
#define __CORRELATIONSSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "static/staticbuilder.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace itensor;
using namespace std;

class StaticCorrelationService
{
private:

	StaticBuilder* staticBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:

	StaticCorrelationService(StaticBuilder* sb, RepositoryBuilder* rb) : staticBuilder(sb), repoBuilder(rb) {} 
	void calculate(Args* args_in)
	{
		auto key =  "magnetizationService";
		auto args = build_args(args_in, key);
		args->add("Static","StaticCorrelation");

		repo = repoBuilder->build(args, key);
		auto corr = staticBuilder->build(args, key);
		auto save = args->getBool("save");
		
		corr->calculate();
		auto [xlabels,xresults] = corr->getResults();
		auto [plabels,presults] = corr->getResults();
		repo->save(StaticCorrelation::getHash(args),"staticcorrelation/x/"+args->getString("Model"),xlabels,xresults,true); //<--- Update
		repo->save(StaticCorrelation::getHash(args),"staticcorrelation/p/"+args->getString("Model"),plabels,presults,true); //<--- Update
	}
};

#endif
