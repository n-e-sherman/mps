#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "model/model.h"
#include "model/heisenberg.h"
#include "model/heisenbergfield.h"
#include "model/heisenberglr.h"
#include "model/xx.h"
#include "model/xxz.h"
#include "model/dimer.h"
#include "sites/sitesbuilder.h"
#include "lattice/latticebuilder.h"

class ModelBuilder
{

protected:
	LatticeBuilder* latticeBuilder;
	SitesBuilder* sitesBuilder;

public:
	ModelBuilder() {}
	ModelBuilder(LatticeBuilder* lb, SitesBuilder* sb) : latticeBuilder(lb), sitesBuilder(sb) {}
	~ModelBuilder(){}
	Model* build(Args* args_in, std::string key = "")
	{
		auto base = "model";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto model = args->getString("Model");	
		cout << "building model: " << model << " -- key: " << key << endl;
		if(model == "Heisenberg") 
		{
 			return new Heisenberg(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "HeisenbergField") 
		{
			return new HeisenbergField(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "HeisenbergLR") 
		{
			return new HeisenbergLR(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "XX") 
		{
			return new XX(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "XXZ") 
		{
			return new XXZ(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "Dimer") 
		{
			return new Dimer(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else
		{
			/* Implement other models here, may want to use else if */
			return nullptr;
		}
	}
};
#endif