#ifndef __XX_H_
#define __XX_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class XX : public Model
{
protected:

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
		auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
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
        	}
        	if(b.t == Lattice::environment)
        	{
	            ampo += -0.5,"S+",b.s1,"S-",b.s2;
	            ampo += -0.5,"S-",b.s1,"S+",b.s2;
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
        			auto hterm = 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm +=     0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
                    hterm +=     0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
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
        			auto hterm = 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm +=     0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = 0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm +=     0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t == Lattice::environment)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = -0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm +=     -0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -0.5*op(sites->getSites(),"S+",b.s1)*op(sites->getSites(),"S-",b.s1+1);
        			hterm +=     -0.5*op(sites->getSites(),"S-",b.s1)*op(sites->getSites(),"S+",b.s1+1);
        			gates.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
        return gates;
	}

public:

	XX(Args* a, Lattice* l, Sites* s) : Model(a,l,s) {}

};
#endif
