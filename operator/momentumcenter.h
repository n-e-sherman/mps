#ifndef __MOMENTUMCENTER_H_
#define __MOMENTUMCENTER_H_

#include "operator/operator.h"
#include "lattice/lattice.h"
#include "operator/momentum.h"
#include <functional>

class MomentumCenter : public Momentum

{

public:
	MomentumCenter(Args* a, Sites* s, Lattice* l) : Momentum(a,s,l,true) { _build(); }

protected:

	virtual void _build_O(Real coeff)
	{
		auto qx = args->getReal("qx");
		auto qy = args->getReal("qy");
		auto qz = args->getReal("qz");
		auto s_op = args->getString("localOperator");
		auto thermal = args->getBool("thermal");
		auto N = args->getInt("N");
		auto c = ((thermal) ? N-1 : N/2);
		vector<Real> rc;
		for(auto s : lattice->getSites())
        {
        	if(s.t == Lattice::physical)
        	{
        		if(s.s == c)
        		{
        			rc = vector<Real>{s.rx,s.ry,s.rz};
        		}
	        }
    	}
        auto ampo = AutoMPO(sites->getSites());
        for(auto s : lattice->getSites())
        {
        	if(s.t == Lattice::physical)
        	{
        		ampo += coeff*func((s.rx - rc[0])*qx + (s.ry-rc[1])*qy + (s.rz-rc[2])*qz),s_op,s.s;
	        }
    	}
    	O = toMPO(ampo);
	}

};
#endif