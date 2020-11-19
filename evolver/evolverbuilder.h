#ifndef __EVOLVERBUILDER_H_
#define __EVOLVERBUILDER_H_

/* This needs some serious love, deal with this later */

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "model/modelbuilder.h"
#include "evolver/evolver.h"
#include "evolver/expmpo.h"
#include "evolver/trotter.h"

using namespace itensor;
using namespace std;

class EvolverBuilder
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;

public:

	EvolverBuilder(ModelBuilder* mb) : modelBuilder(mb){}
	~EvolverBuilder() {}	
	Evolver* build(Args* a)
	{
		cout << "building evolver" << endl;
		args = a;
		auto evolver = args->getString("Evolver");
		if(evolver == "Trotter")
		{
			return new Trotter(args, modelBuilder->build(args));
		}
		else
		if(evolver == "ExpMPO")
		{
			return new ExpMPO(args, modelBuilder->build(args));
		}
		else
		{
			/* Implement other operators here, may want to use else if. */
			return nullptr;
		}

	}
};
#endif