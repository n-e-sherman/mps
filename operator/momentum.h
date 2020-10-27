#ifndef __MOMENTUM_H_
#define __MOMENTUM_H_

#include "operator/operator.h"

class Momentum : public Operator
{
protected:
	/* Inputs */
	Lattice* lattice;
	SiteSet sites;
	int N;
	bool thermal;

public:
	Momentum(){}
	Momentum(Args* a) : Operator(a) {}
	Momentum(Args* a, Lattice* l, SiteSet s) : Operator(a) 
	{
		lattice = l;
		sites = s;
		auto N = args->getInt("N");
		thermal = args->getBool("thermal");
	}
	~Momentum(){}

	MPO& build(Real qfactor)
	{
		auto s_op = args->getString("localOperator");
        auto q = qfactor*M_PI*(Real(N)/Real(N+1));
        auto coeff = sqrt(2.0/(Real(N+1)));
        auto ampo = AutoMPO(sites);
        for(auto s : lattice->getSites())
        {
        	if(s.t == Lattice::physical)
        	{
        		if(thermal)
        		{
	            	ampo += coeff*sin(0.5*(s.s+1)*q),s_op,s.s;
        		}
	            else
	            	ampo += coeff*sin(s.s*q),s_op,s.s;
	        }
    	}
    	O = toMPO(ampo);
    	return O;
    }
};
#endif