#ifndef __EVOLVERBUILDER_H_
#define __EVOLVERBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "model/modelbuilder.h"
#include "evolver/evolver.h"
#include "evolver/expmpo.h"
#include "evolver/trotter.h"
#include "evolver/tdvp.h"

using namespace itensor;
using namespace std;

class EvolverBuilder
{
protected:

	ModelBuilder* modelBuilder;
	Model* model;

public:

	EvolverBuilder(ModelBuilder* mb) : modelBuilder(mb){}
	~EvolverBuilder() {}	
	Evolver* build(Args* args)
	{
		auto evolver = args->getString("Evolver");
		cout << "building evolver: " << evolver << endl;
		
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
		if(evolver == "TDVP")
		{
			return new TDVP(args, modelBuilder->build(args));
		}
		else
		{/* Implement other operators here, may want to use else if. */
			return nullptr;
		}

	}
};
#endif