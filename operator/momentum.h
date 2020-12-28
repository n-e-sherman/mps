#ifndef __MOMENTUM_H_
#define __MOMENTUM_H_

#include "operator/operator.h"
#include "lattice/lattice.h"

class Momentum : public Operator

{
protected:
    Lattice* lattice;
public:
	Momentum(Args* a, Sites* s, Lattice* l) : Operator(a,s), lattice(l) { _build(); }

private:
	void _build()
	{
		auto N = args->getInt("N");
		auto qfactor = args->getReal("qFactor");
		auto s_op = args->getString("localOperator");
        auto q = qfactor*M_PI*(Real(N)/Real(N+1));
        auto coeff = sqrt(2.0/(Real(N+1)));
        auto ampo = AutoMPO(sites->getSites());
        for(auto s : lattice->getSites())
        {
        	if(s.t == Lattice::physical)
        	{
        		ampo += coeff*sin(s.p*q),s_op,s.s;
	        }
    	}
    	O = toMPO(ampo);
	}

};
#endif