#ifndef __XXZ_H_
#define __XXZ_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class XXZ : public Model
{
	Real Jxy;
	Real Delta;
protected:

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
		auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5*Jxy,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5*Jxy,"S-",b.s1,"S+",b.s2;
	            ampo +=   Delta,"Sz",b.s1,"Sz",b.s2;
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
	    	if(b.t == Lattice::physical)
	    	{
	            ampo += 0.5*Jxy,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5*Jxy,"S-",b.s1,"S+",b.s2;
	            ampo +=   Delta,"Sz",b.s1,"Sz",b.s2;
	    	}
	    	if(b.t == Lattice::environment)
	    	{
	            ampo +=   -0.5*Jxy,"S+",b.s1,"S-",b.s2;
	            ampo +=   -0.5*Jxy,"S-",b.s1,"S+",b.s2;
	            ampo += -1.0*Delta,"Sz",b.s1,"Sz",b.s2;
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
        			auto hterm = Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
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
        			auto hterm = Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t == Lattice::environment)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = -Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -Delta*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*Jxy*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*Jxy*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
		return gates;
	}

public:

	XXZ(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }
    virtual void setParams() 
    {
		Jxy = args->getReal("Jxy");
		Delta = args->getReal("Delta");
		params["Jxy"] = Jxy;
		params["Delta"] = Delta;
	}

};
#endif
