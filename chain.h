#ifndef __CHAIN_H_
#define __CHAIN_H_
#include <vector>
#include "service.h"
#include "itensor/all.h"
#include "lattice.h"

using namespace std;
using namespace itensor;

class Chain : public Lattice
{

protected:


public:
	Chain(int N){ for(auto i : range1(N-1)) bonds.push_back(bond(i,i+1,physical)); }
};

#endif