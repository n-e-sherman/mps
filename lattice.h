#ifndef __LATTICE_H_
#define __LATTICE_H_
#include <vector>
#include "itensor/all.h"

using namespace std;
using namespace itensor;

class Lattice
{
protected:
	vector<int> physical;
	vector<int> auxiliary;


public:
	Lattice(int N, bool T)
	{
		if(T)
		{
			for(auto i : range(N)) physical.push_back(2*i);
			for(auto i : range(N)) auxiliary.push_back(2*i+1);
		}
		else
			for(auto i : range(N)) physical.push_back(2*i);
	} // not general, but the template.
	
	vector<int>*
	getPhysical() {return &physical;}

	vector<int>*
	getAuxiliary() {return &auxiliary;}



};

#endif