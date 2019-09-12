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
        }
        H = toMPO(ampo);
	}
public:
	Heisenberg(){}
	Heisenberg(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		calcH();
	}
	~Heisenberg(){}
	static string getHash(Lattice* l, SiteSet s)
	{
		return "Heisenberg_" + Model::getHash(l,s); // This is not unique if you do fermions, may have to change later.
	}
	virtual string getHash() {return Heisenberg::getHash(lattice,sites); }
};
#endif
