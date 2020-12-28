#ifndef __DIMER_H_
#define __DIMER_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class Dimer : public Model
{
	Real Je;
	Real Jo;
protected:

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
        auto ampo = AutoMPO(sites->getSites());
            for(auto b : *lattice)
            {
            	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
            	if(b.t == Lattice::physical)
            	{
            		if(b.z == 0) // Even
            		{
    		            ampo += 0.5*Je,"S+",b.s1,"S-",b.s2;
    		            ampo += 0.5*Je,"S-",b.s1,"S+",b.s2;
    		            ampo +=     Je,"Sz",b.s1,"Sz",b.s2;
    	        	}
            		else // Odd
            		{
    		            ampo += 0.5*Jo,"S+",b.s1,"S-",b.s2;
    		            ampo += 0.5*Jo,"S-",b.s1,"S+",b.s2;
    		            ampo +=     Jo,"Sz",b.s1,"Sz",b.s2;
    	        	}
            	}
            }
            if(shift != 0) calcAmpoShift(ampo, shift);
            return ampo;
	}

	virtual AutoMPO calcAmpoL(bool skip = false, Real shift = 0)
	{
	    auto ampo = AutoMPO(sites->getSites());
	    for(auto b : *lattice)
	    {
	    	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
        	if(b.t == Lattice::physical)
        	{
        		if(b.z == 0) // Even
        		{
		            ampo += 0.5*Je,"S+",b.s1,"S-",b.s2;
		            ampo += 0.5*Je,"S-",b.s1,"S+",b.s2;
		            ampo +=     Je,"Sz",b.s1,"Sz",b.s2;
	        	}
        		else // Odd
        		{
		            ampo += 0.5*Jo,"S+",b.s1,"S-",b.s2;
		            ampo += 0.5*Jo,"S-",b.s1,"S+",b.s2;
		            ampo +=     Jo,"Sz",b.s1,"Sz",b.s2;
	        	}
        	}
        	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
	    	if(b.t == Lattice::environment)
        	{
        		if(b.z == 0) // Even
        		{
		            ampo += -0.5*Je,"S+",b.s1,"S-",b.s2;
		            ampo += -0.5*Je,"S-",b.s1,"S+",b.s2;
		            ampo +=     -Je,"Sz",b.s1,"Sz",b.s2;
	        	}
        		else // Odd
        		{
		            ampo += -0.5*Jo,"S+",b.s1,"S-",b.s2;
		            ampo += -0.5*Jo,"S-",b.s1,"S+",b.s2;
		            ampo +=     -Jo,"Sz",b.s1,"Sz",b.s2;
	        	}
        	}
	    }
        if(shift != 0) calcAmpoShift(ampo, shift);
        return ampo;
	}

	vector<gate> calcGatesH()
	{
        vector<gate> gates;
		for(auto b : *lattice)
		{
        	if(b.t == Lattice::physical)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = Je*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Je*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Je*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Jo*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jo*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jo*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
        return gates;
	}

	vector<gate> calcGatesL()
	{
        vector<gate> gates;
		for(auto b : *lattice)
		{
        	if(b.t == Lattice::physical)
        	{
        		if(b.z == 0) // Even
        		{
        			auto hterm = Je*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Je*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Je*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Jo*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jo*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jo*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t ==Lattice::environment)
        	{
        		if(b.z == 0) // Even
        		{
        			auto hterm = -Je*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*Je*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*Je*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -Jo*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*Jo*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*Jo*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
        return gates;
	}


public:

	Dimer(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }

private:
    
    virtual void setParams()
    {
            Je = args->getReal("Je");
            Jo = args->getReal("Jo");
            params["Je"] = Je;
            params["Jo"] = Jo;
    }
    
};
#endif
