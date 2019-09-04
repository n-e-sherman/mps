#ifndef __LATTICEBUILDER_H_
#define __LATTICEBUILDER_H_
#include "infrastructure/builder.h"
#include "lattice/thermalchain.h"
#include "lattice/chain.h"
/* Add lattice types in include. */


class LatticeBuilder : public Builder
{
public:
	LatticeBuilder(){}
	~LatticeBuilder(){}
	Lattice* build(InputGroup* input)
	{
		auto N = input->getInt("N",100);
		auto thermal = input->getYesNo("thermal",0);
		auto lattice = input->getString("lattice","chain");
		Lattice* L;
		if(thermal)
		{
			if(lattice == "chain") L = new ThermalChain(N);
		}
		else
		{
			if(lattice == "chain") L = new Chain(N);
		}
		return L;
		// TODO: You have no validation that L will be created.
		// TODO: Add other lattices here. Maybe write a function that checks and builds for cleaner code?
	}
};
#endif