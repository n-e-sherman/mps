#ifndef __KZMSERVICE_H_
#define __KZMSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "kzm/kzmbuilder.h"

using namespace itensor;
using namespace std;

class KZMService
{
private:

	KZMBuilder* kzmBuilder;
	KZM* kzm;
	RepositoryBuilder* repoBuilder;
	Repository* repo;


public:

	KZMService(KZMBuilder* kb, RepositoryBuilder* rb) : kzmBuilder(kb), repoBuilder(rb) {}

	void calculate(Args* args_in)
	{
		auto key =  "kzmService";
		auto args = build_args(args_in, key);

		repo = repoBuilder->build(args, key);
		kzm = kzmBuilder->build(args, key); // <-- Issue here
		auto save = args->getBool("save");

		Real t_f = (args->getReal("g0") - args->getReal("gf")) / args->getReal("v");
		while(kzm->getTime() < t_f)
		{
			kzm->calculate();
			repo->save(KZM::getHash(args),kzm,save);
			auto [labels,results] = kzm->getResults();
			repo->save(KZM::getHash(args),"kzm",labels,results);
		}
		auto [labels,results] = kzm->getResults();
		repo->save(KZM::getHash(args),"kzm",labels,results);
	}
};

#endif
