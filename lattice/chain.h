#ifndef __CHAIN_H_
#define __CHAIN_H_
#include <vector>
#include "infrastructure/service.h"
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
		for(auto i : range1(N-1)) bonds.push_back(bond{i,i+1,physical}); 
		for(auto i : range1(N)) sites.push_back(site{i,physical}); 
	}
	static string getHash(int N)
	{
		return "Chain_" + Lattice::getHash(N);
	}
	virtual string getHash(){ return Chain::getHash(N); }
};

#endif