#ifndef __XX_H_
#define __XX_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class XX : public Model
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
        	}
        	if(b.t == Lattice::environment)
        	{
	            *ampoL += -0.5,"S+",b.s1,"S-",b.s2;
	            *ampoL += -0.5,"S-",b.s1,"S+",b.s2;
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
        			auto hterm = 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm +=     0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesH.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
                    hterm +=     0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
                    gatesH.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
        cout << "gates calculated." << endl;
	}

	void calcGatesL()
	{
		for(auto b : *lattice)
		{
        	if(b.t == Lattice::physical)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm +=     0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = 0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm +=     0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
        	if(b.t == Lattice::environment)
        	{

        		if(b.z == 0) // Even
        		{
        			auto hterm = -0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm +=     -0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"even"});
	        	}
        		else // Odd
        		{
        			auto hterm = -0.5*op(sites,"S+",b.s1)*op(sites,"S-",b.s1+1);
        			hterm +=     -0.5*op(sites,"S-",b.s1)*op(sites,"S+",b.s1+1);
        			gatesL.push_back(gate{b.s1,b.s1+1,hterm,"odd"});
	        	}
        	}
		}
	}

    /* Gates for H */
    /* Gates for L */

public:
	XX(){}
	XX(Args* a, Lattice* l) : Model(a,l) {}
	XX(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) {}
	~XX(){}
};
#endif
