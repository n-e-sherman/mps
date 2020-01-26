#ifndef __CHAIN_H_
#define __CHAIN_H_
#include <vector>
#include "itensor/all.h"
#include "lattice/lattice.h"

using namespace std;
using namespace itensor;

class Chain : public Lattice
{
public:
	Chain(){}
	Chain(int L) : Lattice(L) 
	{
		for(auto i : range1(N-1)) bonds.push_back(bond{i,i+1,physical,i%2}); 
		for(auto i : range1(N)) sites.push_back(site{i,physical}); 
	}
};

#endif