#ifndef __LATTICEBUILDER_H_
#define __LATTICEBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "lattice/thermalchain.h"
#include "lattice/chain.h"
#include "lattice/lattice.h"

class LatticeBuilder
{
public:

	Lattice* build(Args* args)
	{
		auto thermal = args->getBool("thermal");
		auto lattice = args->getString("Lattice");
		cout << "building lattice: " << lattice << endl;
		
		if(lattice == "Chain")
		{
			if(thermal)
				return new ThermalChain(args);
			return new Chain(args);
		}
		else
		{
			/* Implement other lattices here, may want to use else if. */
			return nullptr;
		}
	}
};
#endif