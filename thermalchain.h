#ifndef __THERMALCHAIN_H_
#define __THERMALCHAIN_H_
#include <vector>
#include "service.h"
#include "itensor/all.h"
#include "lattice.h"

using namespace std;
using namespace itensor;

class ThermalChain : public Lattice
{
protected:


public:
	ThermalChain(int N)
	{ 
		for(auto i : range1(N-1)) bonds.push_back(bond(2*i,2*(i+1),physical));
		for(auto i : range1(N-1)) bonds.push_back(bond(2*i+1,2*(i+1)+1,environment));
	}

};

#endif