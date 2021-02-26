#ifndef __STATEBUILDER_H_
#define __STATEBUILDER_H_


#include "itensor/all.h"
#include "infrastructure/util.h"
#include "state/state.h"
#include "state/groundstate.h"
#include "state/thermalstate.h"
#include "model/modelbuilder.h"
#include "lattice/latticebuilder.h"
#include "evolver/evolverbuilder.h"



using namespace itensor;
using namespace std;

class StateBuilder
{
protected:

	ModelBuilder* modelBuilder;
	LatticeBuilder* latticeBuilder;
	EvolverBuilder* evolverBuilder;

public:

	StateBuilder(ModelBuilder* mb, LatticeBuilder* lb, EvolverBuilder* eb) : 
				 modelBuilder(mb), latticeBuilder(lb), evolverBuilder(eb) {}

	State* build(Args* args_in, std::string key = "")
	{
		auto base = "state";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto thermal = args->getBool("thermal");
		cout << "building state: ";
		if(thermal)
		{
			cout << "ThermalState" << " -- key: " << key << endl;
			auto swap = args->getBool("swap");
			args->add("swap",true);
			auto res = new ThermalState(args,evolverBuilder->build(args, key));
			args->add("swap",swap);
			return res;
		}
		else
		{
			cout << "GroundState" << " " << key << endl;
			return new GroundState(args, modelBuilder->build(args, key)); 
		}
	}
};
#endif