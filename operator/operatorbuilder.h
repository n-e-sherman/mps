#ifndef __OPERATORBUILDER_H_
#define __OPERATORBUILDER_H_

/* This needs some serious love, deal with this later */

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "operator/operator.h"
#include "operator/momentum.h"

using namespace itensor;
using namespace std;

class OperatorBuilder
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	SiteSet sites;
	LatticeBuilder* latticeBuilder;
	Lattice* lattice;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:

	OperatorBuilder() {}
	OperatorBuilder(ModelBuilder* mb, LatticeBuilder* lb, RepositoryBuilder* rb) {modelBuilder = mb; repoBuilder = rb; latticeBuilder = lb; }
	~OperatorBuilder() {}	
	Operator* build(Args* a)
	{
		cout << "building operator" << endl;
		args = a;
		repo = repoBuilder->build(args);
		lattice = latticeBuilder->build(args);
		model = modelBuilder->build(args);
		sites = model->getSites();
		auto op = args->getString("Operator");
		if(op == "Momentum")
		{
			return new Momentum(args,lattice,sites);
		}
		else
		{
			/* Implement other operators here, may want to use else if. */
			return nullptr;
		}

	}
};
#endif