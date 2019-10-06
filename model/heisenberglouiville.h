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
		auto ampoP = AutoMPO(sites);
		auto ampoE = AutoMPO(sites);
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampoP += 0.5,"S+",b.s1,"S-",b.s2;
	            ampoP += 0.5,"S-",b.s1,"S+",b.s2;
	            ampoP += 1.0,"Sz",b.s1,"Sz",b.s2;
        	}
        	if(b.t == Lattice::environment)
        	{
	            ampoE += 0.5,"S+",b.s1,"S-",b.s2;
	            ampoE += 0.5,"S-",b.s1,"S+",b.s2;
	            ampoE += 1.0,"Sz",b.s1,"Sz",b.s2;
        	}
        }
        auto HP = toMPO(ampoP);
        auto HE = toMPO(ampoE);
        H = sum(HP,-1*HE,*args);
	}
public:
	HeisenbergLouiville(){}
	HeisenbergLouiville(Args* a) : Model(a) {}
	HeisenbergLouiville(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		calcH();
	}
	~HeisenbergLouiville(){}
};
#endif
