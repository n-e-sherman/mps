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
	KZM* build(Args* args)
	{
		repo = repoBuilder->build(args);
		auto load = args->getBool("loadKZM");
		if(load)
		{
			auto kzm = repo->load(KZM::getHash(args), new KZM(args));
			if(kzm != nullptr) return kzm;	
		}
		auto kzm = new KZM(args, SpinHalf(3, {"ConserveQNs=", false})); // Could be generalized
		repo->save(KZM::getHash(args), kzm);
		return kzm;
	}
};
#endif