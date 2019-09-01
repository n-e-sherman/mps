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
		auto ampo = AutoMPO(sites);
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
	            ampo +=     "Sz",b.s1,"Sz",b.s2;
        	}
        	else
        	{
        		/* TODO: Handle Thermal */
        	}
        }
        H = toMPO(ampo);
	}
public:
	Heisenberg(){}
	Heisenberg(Lattice* l, SiteSet s, InputGroup* i) : Model(l,s,i) 
	{
		calcH();
	}
	~Heisenberg(){}
};
#endif
