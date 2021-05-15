#ifndef __EVOLVERBUILDER_H_
#define __EVOLVERBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "model/modelbuilder.h"
#include "evolver/evolver.h"
#include "evolver/expmpo.h"
#include "evolver/trotter.h"
#include "evolver/trivial.h"
#include "evolver/tdvp.h"
#include "evolver/mixed.h"

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
	Evolver* build(Args* args_in, std::string key = "")
	{
		auto base = "evolver";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto evolver = args->getString("Evolver");
		cout << "building evolver: " << evolver << " -- key: " << key << endl;
		if(evolver == "Trivial")
		{
			return new Trivial(args);
		}
		if(evolver == "Trotter")
		{
			return new Trotter(args, modelBuilder->build(args, key));
		}
		else
		if(evolver == "ExpMPO")
		{
			return new ExpMPO(args, modelBuilder->build(args, key));
		}
		else
		if(evolver == "TDVP")
		{
			if(args->getBool("swap"))
				return new TDVPswap(args, modelBuilder->build(args, key));	
			return new TDVP(args, modelBuilder->build(args, key));
		}
		else
		if(evolver == "Mixed")
		{
			cout << "Mixed Evolver is currently out of service." << endl;
			exit(1);
			return new Mixed(args, modelBuilder->build(args, key));
		}
		else
		{/* Implement other operators here, may want to use else if. */
			return nullptr;
		}

	}
};
#endif