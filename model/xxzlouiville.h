#ifndef __XXZLOUIVILLE_H_
#define __XXZLOUIVILLE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class XXZLouiville : public Model
{
	Real Delta;
protected:
	virtual void calcH()
	{
	    for(auto b : *lattice)
	    {
	    	if(b.t == Lattice::physical)
	    	{
	            ampo += 0.5*Delta,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5*Delta,"S-",b.s1,"S+",b.s2;
	            ampo += 1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    	if(b.t == Lattice::environment)
	    	{
	            ampo += -0.5*Delta,"S+",b.s1,"S-",b.s2;
	            ampo += -0.5*Delta,"S-",b.s1,"S+",b.s2;
	            ampo += -1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    }
	}
public:
	XXZLouiville(){}
	XXZLouiville(Args* a) : Model(a) {}
	XXZLouiville(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		Delta = args->getReal("Delta");
		calcH();
        H = toMPO(ampo);
	}
	~XXZLouiville(){}
};
#endif
