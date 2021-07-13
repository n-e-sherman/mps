#ifndef __MOMENTUM_H_
#define __MOMENTUM_H_

#include "operator/operator.h"
#include "lattice/lattice.h"
#include <functional>

class Momentum : public Operator

{
protected:
    Lattice* lattice;
    std::function<Real(Real)> func;
public:
	Momentum(Args* a, Sites* s, Lattice* l) : Operator(a,s), lattice(l) { _build(); }

private:
	void _build()
	{
		auto N = args->getInt("N");
		auto s_op = args->getString("localOperator");
		auto qfunc = args->getString("momentumFunction");
		auto coeff = 1.0;
		if (qfunc == "sin")
		{
			func = _sin;
			coeff = sqrt(2.0/(Real(N+1)));
		}
		else
		if (qfunc == "cos")
		{
			func = _cos;
			coeff = sqrt(1.0/(Real(N)));
		}
		auto qx = args->getReal("qx");
		auto qy = args->getReal("qy");
		auto qz = args->getReal("qz");

		// auto qfactor = args->getReal("qFactor");
        // auto q = qfactor*M_PI*(Real(N)/Real(N+1));
        // auto coeff = sqrt(2.0/(Real(N+1)));

        auto ampo = AutoMPO(sites->getSites());
        for(auto s : lattice->getSites())
        {
        	if(s.t == Lattice::physical)
        	{
        		ampo += coeff*func(s.rx*qx + s.ry*qy + s.rz*qz),s_op,s.s;
	        }
    	}
    	O = toMPO(ampo);
	}

	static Real _sin(Real x)
	{
		return sin(x);
	}
	static Real _cos(Real x)
	{
		return cos(x);
	}
};
#endif