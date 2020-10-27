#ifndef __HEISENBERGFIELD_H_
#define __HEISENBERGFIELD_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class HeisenbergField : public Model
{
    Real B;
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

        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    *ampoH +=   B,"Sz",s.s;    
                }    
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

        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    *ampoL +=   B,"Sz",s.s;    
                }
                if(s.t == Lattice::environment)
                {
                    *ampoL +=   -B,"Sz",s.s;    
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
                auto C1 = (4.0/3.0)*B;
                auto C2 = (4.0/3.0)*B;
                if(b.s1 == 1) C1 *= 2;
                if(b.s2 == (2*args->getInt("N") - 1) ) C2 *= 2;
        		if(b.z == 0) // Even
        		{
        			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    hterm +=  C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
        			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    hterm +=  C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
        			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
	}

	void calcGatesL()
	{
		for(auto b : *lattice)
		{
            auto C1 = (4.0/3.0)*B;
            auto C2 = (4.0/3.0)*B;
            if(b.s1 <= 2) C1 *= 2;
            if(b.s2 >= (2*args->getInt("N") - 1)) C2 *= 2;
        	if(b.t == Lattice::physical)
        	{
                
        		if(b.z == 0) // Even
        		{
        			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    hterm +=  C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += 0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    hterm +=  C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
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
                    hterm +=  -C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  -C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -op(sites,"Sz",b.s1)*op(sites,"Sz",b.s1+1);
        			hterm += -0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm += -0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    hterm +=  -C1*op(sites,"Sz",b.s1)*op(sites,"S2",b.s1+1);
                    hterm +=  -C2*op(sites,"S2",b.s1)*op(sites,"Sz",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
	}

public:
	HeisenbergField(){}
	HeisenbergField(Args* a, Lattice* l) : Model(a,l) { setParams(); }
	HeisenbergField(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) { setParams(); }
    virtual void setParams()
    {
        B = args->getReal("B");
        params["B"] = B;
    }
	~HeisenbergField(){}
};
#endif
