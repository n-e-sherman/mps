#ifndef __CHAIN_H_
#define __CHAIN_H_

#include "lattice/lattice.h"

using namespace std;
using namespace itensor;

class Chain : public Lattice
{
public:
	
	Chain(Args* a) : Lattice(a) 
	{	
		auto N = args->getInt("N");
		for(int i : range1(N-1)) bonds.push_back(bond{i,i+1,physical,i%2}); 
		for(int i : range1(N)) sites.push_back(site{i,i,0,physical}); 
	}
};

#endif