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

	virtual void calcAmpoH()
	{
		ampoH = AutoMPO(sites);
                for(auto b : *lattice)
                {
                	if(b.t == Lattice::physical)
                	{
        	            *ampoH += 0.5,"S+",b.s1,"S-",b.s2;
        	            *ampoH += 0.5,"S-",b.s1,"S+",b.s2;
        	            *ampoH +=     "Sz",b.s1,"Sz",b.s2;
                	}
                }
	}
	
	virtual void calcAmpoL()
	{
            ampoL = AutoMPO(sites);
	    for(auto b : *lattice)
	    {
	    	if(b.t == Lattice::physical)
	    	{
	            *ampoL += 0.5,"S+",b.s1,"S-",b.s2;
	            *ampoL += 0.5,"S-",b.s1,"S+",b.s2;
	            *ampoL += 1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    	if(b.t == Lattice::environment)
	    	{
	            *ampoL += -0.5,"S+",b.s1,"S-",b.s2;
	            *ampoL += -0.5,"S-",b.s1,"S+",b.s2;
	            *ampoL += -1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    }
	}

	void calcGatesH()
	{
		for(auto b : *lattice)
		{
                	if(b.t == Lattice::physical)
                	{

                		if(b.z == 0) // Even
                		{
                			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
        	        	}
                	}
		}
	}

	void calcGatesL()
	{
		for(auto b : *lattice)
		{
                	if(b.t == Lattice::physical)
                	{

                		if(b.z == 0) // Even
                		{
                			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
        	        	}
                	}
                	if(b.t == Lattice::environment)
                	{

                		if(b.z == 0) // Even
                		{
                			auto hterm = -op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += -0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += -0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = -op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += -0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += -0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
        	        	}
                	}
		}
	}

public:
	Heisenberg(){}
	Heisenberg(Args* a) : Model(a) {}
	Heisenberg(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) {}
	~Heisenberg(){}
};
#endif
