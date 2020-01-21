#ifndef __HEISENBERG_H_
#define __HEISENBERG_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Heisenberg : public Model
{
protected:
	virtual void calcH()
	{
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
	            ampo +=     "Sz",b.s1,"Sz",b.s2;
        	}
        }
	}
public:
	Heisenberg(){}
	Heisenberg(Args* a) : Model(a) {}
	Heisenberg(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		calcH();
        H = toMPO(ampo);
	}
	~Heisenberg(){}
};
#endif
