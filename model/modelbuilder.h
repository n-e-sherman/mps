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
#include "model/sitebuilder.h"
#include "repository/repositorybuilder.h"

class ModelBuilder
{
public:
	enum modelType
	{
		normal = 0,
		thermal = 1
	};
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
	Model* build(Args* a, modelType mType = normal)
	{
		cout << "building model" << endl;
		args = a;
		repo = repoBuilder->build(args);
		auto modelName = args->getString("Model");
		if(modelName == "Heisenberg") 
		{
			if(mType == normal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new Heisenberg(args));
				if(model != nullptr) return model;
				model = new Heisenberg(args, latticeBuilder->build(args), siteBuilder->build(args));
				if(args->getBool("squared")) model->squared();
				repo->save(Model::getHash(args,mType), model);
				return model;
			}
			else if(mType == thermal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new HeisenbergLouiville(args));
				if(model != nullptr) return model;
				model = new HeisenbergLouiville(args, latticeBuilder->build(args), siteBuilder->build(args));
				if(args->getBool("squared")) model->squared();
				repo->save(Model::getHash(args,mType), model);
				return model;
			}
		}
		else
		if(modelName == "XX") 
		{
			if(mType == normal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new XX(args));
				if(model != nullptr) return model;
				model = new XX(args, latticeBuilder->build(args), siteBuilder->build(args));
				if(args->getBool("squared")) model->squared();
				repo->save(Model::getHash(args,mType), model);
				return model;
			}
			else if(mType == thermal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new XXLouiville(args));
				if(model != nullptr) return model;
				model = new XXLouiville(args, latticeBuilder->build(args), siteBuilder->build(args));
				if(args->getBool("squared")) model->squared();
				repo->save(Model::getHash(args,mType), model);
				return model;
			}
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