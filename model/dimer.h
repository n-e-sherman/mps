#ifndef __DIMER_H_
#define __DIMER_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Dimer : public Model
{
	Real Je;
	Real Jo;
protected:

	virtual void calcAmpoH()
	{
		ampoH = AutoMPO(sites);
                for(auto b : *lattice)
                {
                	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
                	if(b.t == Lattice::physical)
                	{
                		if(b.z == 0) // Even
                		{
        		            *ampoH += 0.5*Je,"S+",b.s1,"S-",b.s2;
        		            *ampoH += 0.5*Je,"S-",b.s1,"S+",b.s2;
        		            *ampoH +=     Je,"Sz",b.s1,"Sz",b.s2;
        	        	}
                		else // Odd
                		{
        		            *ampoH += 0.5*Jo,"S+",b.s1,"S-",b.s2;
        		            *ampoH += 0.5*Jo,"S-",b.s1,"S+",b.s2;
        		            *ampoH +=     Jo,"Sz",b.s1,"Sz",b.s2;
        	        	}
                	}
                }
	}

	virtual void calcAmpoL()
	{
	    ampoL = AutoMPO(sites);
	    for(auto b : *lattice)
	    {
	    	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
        	if(b.t == Lattice::physical)
        	{
        		if(b.z == 0) // Even
        		{
		            *ampoL += 0.5*Je,"S+",b.s1,"S-",b.s2;
		            *ampoL += 0.5*Je,"S-",b.s1,"S+",b.s2;
		            *ampoL +=     Je,"Sz",b.s1,"Sz",b.s2;
	        	}
        		else // Odd
        		{
		            *ampoL += 0.5*Jo,"S+",b.s1,"S-",b.s2;
		            *ampoL += 0.5*Jo,"S-",b.s1,"S+",b.s2;
		            *ampoL +=     Jo,"Sz",b.s1,"Sz",b.s2;
	        	}
        	}
        	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
	    	if(b.t == Lattice::environment)
        	{
        		if(b.z == 0) // Even
        		{
		            *ampoL += -0.5*Je,"S+",b.s1,"S-",b.s2;
		            *ampoL += -0.5*Je,"S-",b.s1,"S+",b.s2;
		            *ampoL +=     -Je,"Sz",b.s1,"Sz",b.s2;
	        	}
        		else // Odd
        		{
		            *ampoL += -0.5*Jo,"S+",b.s1,"S-",b.s2;
		            *ampoL += -0.5*Jo,"S-",b.s1,"S+",b.s2;
		            *ampoL +=     -Jo,"Sz",b.s1,"Sz",b.s2;
	        	}
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
                			auto hterm = Je*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*Je*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*Je*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = Jo*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*Jo*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*Jo*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
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
                			auto hterm = Je*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*Je*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*Je*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = Jo*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += 0.5*Jo*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += 0.5*Jo*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
        	        	}
                	}
                	if(b.t ==Lattice::environment)
                	{
                		if(b.z == 0) // Even
                		{
                			auto hterm = -Je*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += -0.5*Je*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += -0.5*Je*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
        	        	}
                		else // Odd
                		{
                			auto hterm = -Jo*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
                			hterm += -0.5*Jo*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                			hterm += -0.5*Jo*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
        	        	}
                	}
		}
	}


public:
	Dimer(){}
	Dimer(Args* a) : Model(a) {}
	Dimer(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		Je = args->getReal("Je");
		Jo = args->getReal("Jo");
		params["Je"] = Je;
		params["Jo"] = Jo;

	}
	~Dimer(){}
};
#endif
