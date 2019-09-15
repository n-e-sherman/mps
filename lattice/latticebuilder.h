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
	LatticeBuilder(){}
	~LatticeBuilder(){}
	Lattice* build(Args* args)
	{
		/* TODO: Maybe implement repo, kind of overkill though. */
		auto N = args->getInt("N");
		auto thermal = args->getBool("thermal");
		auto latticeName = args->getString("Lattice");
		if(latticeName == "Chain")
		{
			if(thermal)
			{
				return new ThermalChain(N);
			}
			else
			{
				return new Chain(N);
			}
		}
		else
		{
			cout << "This should never be called Lattice..." << endl;
			return nullptr;
			/* Implement other lattices here, may want to use else if. */
		}
	}
};
#endif