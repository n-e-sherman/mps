#ifndef __HEISENBERGLADDER_H_
#define __HEISENBERGLADDER_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class HeisenbergLadder : public Model
{
	Real J1; // Along the chain
	Real J2; // Steps of the ladder
	// Real J3;
	// Real J4;

protected:

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
	    auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
        		if((b.n == 1) & (J1 != 0.0)) // 1st neighbor
        		{
        			ampo += 0.5*J1,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J1,"S-",b.s1,"S+",b.s2;
	                ampo +=     J1,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += 0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo +=     J2,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        }
        if(shift != 0) calcAmpoShift(ampo, shift);
        return ampo;
	}
	
	virtual AutoMPO calcAmpoL(bool skip = false, Real shift = 0)
	{
        Error("CalcAmpoL has not been implented for HeisenbergLR");
        auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
        		if((b.n == 1) & (J1 != 0.0)) // 2nd neighbor
                {
                    ampo += 0.5*J1,"S+",b.s1,"S-",b.s2;
                    ampo += 0.5*J1,"S-",b.s1,"S+",b.s2;
                    ampo +=     J1,"Sz",b.s1,"Sz",b.s2;
                }
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += 0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo +=     J2,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        	if(b.t == Lattice::environment)
        	{
        		if((b.n == 1) & (J1 != 0.0)) // 2nd neighbor
                {
                    ampo += -0.5*J1,"S+",b.s1,"S-",b.s2;
                    ampo += -0.5*J1,"S-",b.s1,"S+",b.s2;
                    ampo += -1.0*J1,"Sz",b.s1,"Sz",b.s2;
                }
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += -0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += -0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo += -1.0*J2,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        }
        if(shift != 0) calcAmpoShift(ampo, shift);
        return ampo;
	}

	vector<gate> calcGatesH()
	{
		vector<gate> gates;
		return gates;
	}

	vector<gate> calcGatesL()
	{
		vector<gate> gates;
		return gates;
	}

public:

	HeisenbergLadder(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }

private:
    
    virtual void setParams()
    {
            J1 = args->getReal("J1");
            J2 = args->getReal("J2");
    }
    
};
#endif
