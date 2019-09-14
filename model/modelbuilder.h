#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_
#include "model/heisenberg.h"
#include "lattice/latticebuilder.h"
#include "model/sitebuilder.h"
#include "model/heisenberglouiville.h"
/* Add lattice types in include. */


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
		args = a;
		repo = repoBuilder->build(args);
		auto modelName = args->getString("Model");
		if(modelName == "Heisenberg") 
		{
			if(mType == normal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new Heisenberg());
				if(model != nullptr) return model;
				model = Heisenberg(args, latticeBuilde->build(args), siteBuilder->build());
				repo.save(Model::getHash(args,mType), model);
				return model;
			}
			else if(mType == thermal) 
			{
				auto model = repo->load(Model::getHash(args,mType), new HeisenbergLouiville());
				if(model != nullptr) return model;
				model = new HeisenbergLouiville(args, latticeBuilde->build(args), siteBuilder->build());
				repo.save(Model::getHash(args,mType), model);
				return model;
			}
		}
		else
		{
			/* Implement other models here, may want to use else if */
			return nullptr;
		}

	}
};
#endif