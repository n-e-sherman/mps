#ifndef __XX_H_
#define __XX_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class XX : public Model
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
        	}
        }
        H = toMPO(ampo);
	}
public:
	XX(){}
	XX(Args* a) : Model(a) {}
	XX(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		calcH();
	}
	~XX(){}
};
#endif
