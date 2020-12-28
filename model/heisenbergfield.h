#ifndef __HEISENBERGFIELD_H_
#define __HEISENBERGFIELD_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class HeisenbergField : public Model
{
protected:
    Real B;
	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
		auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
	            ampo +=     "Sz",b.s1,"Sz",b.s2;
        	}
        }

        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    ampo +=   B,"Sz",s.s;    
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
	    	if(b.t == Lattice::physical)
	    	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
	            ampo += 1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    	if(b.t == Lattice::environment)
	    	{
	            ampo += -0.5,"S+",b.s1,"S-",b.s2;
	            ampo += -0.5,"S-",b.s1,"S+",b.s2;
	            ampo += -1.0,"Sz",b.s1,"Sz",b.s2;
	    	}
	    }

        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    ampo +=   B,"Sz",s.s;    
                }
                if(s.t == Lattice::environment)
                {
                    ampo +=   -B,"Sz",s.s;    
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
                auto C1 = B;
                auto C2 = B;
                if(b.s1 == 1) C1 *= 2;
                if(b.s2 == (2*args->getInt("N") - 1) ) C2 *= 2;
        		if(b.z == 0) // Even
        		{
        			auto hterm = op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
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
            auto C1 = B;
            auto C2 = B;
            if(b.s1 <= 2) C1 *= 2;
            if(b.s2 >= (2*args->getInt("N") - 1)) C2 *= 2;
        	if(b.t == Lattice::physical)
        	{
                
        		if(b.z == 0) // Even
        		{
        			auto hterm = op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += 0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t == Lattice::environment)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = -op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  -C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  -C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			hterm += -0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm += -0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
                    hterm +=  -C1*op(sites->getSites(),"Sz",b.s1)*op(sites->getSites(),"Id",b.s1+1);
                    hterm +=  -C2*op(sites->getSites(),"Id",b.s1)*op(sites->getSites(),"Sz",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
        return gates;
	}

    virtual void setParams()
    {
        B = args->getReal("B");
        params["B"] = B;
    }
public:

	HeisenbergField(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }
    
};
#endif
