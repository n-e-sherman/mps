#ifndef __KZMBUILDER_H_
#define __KZMBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "kzm/kzm.h"
#include "repository/repositorybuilder.h"


class KZMBuilder
{
protected:

	RepositoryBuilder* repoBuilder;
	Repository* repo;


public:

	KZMBuilder(RepositoryBuilder* rb) : repoBuilder(rb) {}					 
	KZM* build(Args* args_in, std::string key = "")
	{
		auto base = "kzm";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);

		repo = repoBuilder->build(args, key);
		auto load = args->getBool("load");

		auto kzm = repo->load(KZM::getHash(args), new KZM(args));
		if((kzm != nullptr) && load) return kzm;	
		kzm = new KZM(args, SpinHalf(3, {"ConserveQNs=", false})); // Could be generalized
		repo->save(KZM::getHash(args), kzm);
		return kzm;
	}
};
#endif