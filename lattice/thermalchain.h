#ifndef __THERMALCHAIN_H_
#define __THERMALCHAIN_H_
#include <vector>
#include "infrastructure/service.h"
#include "itensor/all.h"
#include "lattice/lattice.h"

using namespace std;
using namespace itensor;

class ThermalChain : public Lattice
{
public:
	ThermalChain(){}
	ThermalChain(int L) : Lattice(L)
	{ 
		for(auto i : range1(N-1)) bonds.push_back(bond{2*i-1,2*(i+1)-1,physical});
		for(auto i : range1(N-1)) bonds.push_back(bond{2*i,2*(i+1),environment});
		for(auto i : range1(N)) sites.push_back(site{2*i-1,physical});
		for(auto i : range1(N)) sites.push_back(site{2*i,environment});
	}
	static string getHash(int L)
	{
		return "ThermalChain_" + Lattice::getHash(L);
	}
	virtual string getHash(){ return ThermalChain::getHash(N); }

};

#endif