#ifndef __XXZ_H_
#define __XXZ_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class XXZ : public Model
{
	Real Jz;
	Real Delta;
protected:

	virtual void calcAmpoH()
	{
		ampoH = AutoMPO(sites);
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            *ampoH += 0.5*Delta,"S+",b.s1,"S-",b.s2;
	            *ampoH += 0.5*Delta,"S-",b.s1,"S+",b.s2;
	            *ampoH +=        Jz,"Sz",b.s1,"Sz",b.s2;
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
	            *ampoL += 0.5*Delta,"S+",b.s1,"S-",b.s2;
	            *ampoL += 0.5*Delta,"S-",b.s1,"S+",b.s2;
	            *ampoL +=    1.0*Jz,"Sz",b.s1,"Sz",b.s2;
	    	}
	    	if(b.t == Lattice::environment)
	    	{
	            *ampoL += -0.5*Delta,"S+",b.s1,"S-",b.s2;
	            *ampoL += -0.5*Delta,"S-",b.s1,"S+",b.s2;
	            *ampoL +=    -1.0*Jz,"Sz",b.s1,"Sz",b.s2;
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
        			auto hterm = Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
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
        			auto hterm = Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t == Lattice::environment)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = -Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += -0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += -0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -Jz*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += -0.5*Delta*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += -0.5*Delta*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
	}

public:
	XXZ(){}
	XXZ(Args* a) : Model(a) {}
	XXZ(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		Jz = args->getReal("Jz");
		Delta = args->getReal("Delta");
		params["Jz"] = Jz;
		params["Delta"] = Delta;
	}
	~XXZ(){}
};
#endif
