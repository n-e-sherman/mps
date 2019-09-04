#ifndef __HEISENBERGLOUIVILLE_H_
#define __HEISENBERGLOUIVILLE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class HeisenbergLouiville : public Model
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
        	if(b.t == Lattice::environment)
        	{
	            ampo += -0.5,"S+",b.s1,"S-",b.s2;
	            ampo += -0.5,"S-",b.s1,"S+",b.s2;
	            ampo += -1.0,"Sz",b.s1,"Sz",b.s2;
        	}
        }
        H = toMPO(ampo);
	}
public:
	HeisenbergLouiville(){}
	HeisenbergLouiville(Lattice* l, SiteSet s, InputGroup* i) : Model(l,s,i) 
	{
		calcH();
	}

	MPO const * const getL() const {return &H;}
	~Heisenberg(){}
};
#endif
