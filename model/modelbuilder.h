#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "lattice/latticebuilder.h"
#include "model/model.h"
#include "model/heisenberg.h"
#include "model/heisenberglouiville.h"
#include "model/xx.h"
#include "model/xxlouiville.h"
#include "model/xxz.h"
#include "model/xxzlouiville.h"
#include "model/dimer.h"
#include "model/dimerlouiville.h"
#include "model/sitebuilder.h"
#include "repository/repositorybuilder.h"

class ModelBuilder
{

protected:
	Args* args;
	LatticeBuilder* latticeBuilder;
	SiteBuilder* siteBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
public:
	ModelBuilder() {}
	ModelBuilder(LatticeBuilder* lb, SiteBuilder* sb, RepositoryBuilder* rb) { latticeBuilder = lb; siteBuilder = sb; repoBuilder = rb;}
	~ModelBuilder(){}
	Model* build(Args* a)
	{
		cout << "building model" << endl;
		args = a;
		repo = repoBuilder->build(args);
		auto modelName = args->getString("Model");
		if(modelName == "Heisenberg") 
		{
			// auto model = repo->load(Model::getHash(args,mType), new Heisenberg(args));
			// if(model != nullptr) return model;
			auto model = new Heisenberg(args, latticeBuilder->build(args), siteBuilder->build(args));
			// repo->save(Model::getHash(args,mType), model);
			return model;
		}
		else
		if(modelName == "XX") 
		{
			// auto model = repo->load(Model::getHash(args,mType), new XX(args));
			// if(model != nullptr) return model;
			auto model = new XX(args, latticeBuilder->build(args), siteBuilder->build(args));
			// repo->save(Model::getHash(args,mType), model);
			return model;
		}
		else
		if(modelName == "XXZ") 
		{
			// auto model = repo->load(Model::getHash(args,mType), new XXZ(args));
			// if(model != nullptr) return model;
			auto model = new XXZ(args, latticeBuilder->build(args), siteBuilder->build(args));
			// repo->save(Model::getHash(args,mType), model);
			return model;
		}
		if(modelName == "Dimer") 
		{
			// auto model = repo->load(Model::getHash(args,mType), new Dimer(args));
			// if(model != nullptr) return model;
			auto model = new Dimer(args, latticeBuilder->build(args), siteBuilder->build(args));
			// repo->save(Model::getHash(args,mType), model);
			return model;
		}
		else
		{
			/* Implement other models here, may want to use else if */
			return nullptr;
		}
		return nullptr;

	}
};
#endif