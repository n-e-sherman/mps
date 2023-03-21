#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/cache.h"
#include "model/model.h"
#include "model/heisenberg.h"
#include "model/heisenbergladder.h"
#include "model/heisenbergfield.h"
#include "model/heisenberglr.h"
#include "model/heisenberglrfield.h"
#include "model/heisenberganisotropic.h"
#include "model/xx.h"
#include "model/xxz.h"
#include "model/xxzj1j2.h"
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
		if(model == "HeisenbergLadder") 
		{
			return new HeisenbergLadder(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
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
		if(model == "HeisenbergAnisotropic") 
		{
			auto conserveQNs = args->getBool("ConserveQNs");
			auto conserveSz = args->getBool("ConserveSz");
			if(conserveSz | conserveQNs)
			{
				cout << "HeisenbergAnisotropic cannot conserve symmetries, changing this option" << endl;
				auto cache = Cache::getInstance();
				auto global = (Args*)cache->load("global");
				global->add("ConserveSz", false);
				global->add("ConserveQNs", false);
				cache->save("global", global);
				args->add("ConserveSz", false);
				args->add("ConserveQNs", false);
			}
			return new HeisenbergAnisotropic(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
		}
		else 
		if(model == "HeisenbergLRField") 
		{
			return new HeisenbergLRField(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
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
		if(model == "XXZJ1J2") 
		{
			return new XXZJ1J2(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key));
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