#ifndef __MAGNETIZATIONSERVICE_H_
#define __MAGNETIZATIONSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "static/staticbuilder.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace itensor;
using namespace std;

class MagnetizationService
{
private:

	StaticBuilder* staticBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:

	MagnetizationService(StaticBuilder* sb, RepositoryBuilder* rb) : staticBuilder(sb), repoBuilder(rb) {} 
	void calculate(Args* args_in)
	{
		auto key =  "magnetizationService";
		auto args = build_args(args_in, key);
		args->add("Static","Magnetization");

		repo = repoBuilder->build(args, key);
		auto mag = staticBuilder->build(args, key);
		auto save = args->getBool("save");
		
		mag->calculate();
		auto [labels,results] = mag->getResults();
		repo->save(Magnetization::getHash(args),"magnetization/"+args->getString("Model"),labels,results); //<--- Update
	}
};

#endif
