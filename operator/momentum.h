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

	void updateMomentum(vector<Real> qs)
	{
		if (qs.size() == 1)
			updateMomentum(qs[0]);
		else
		if (qs.size() == 2)
			updateMomentum(qs[0],qs[1]);
		else
		if (qs.size() == 3)
			updateMomentum(qs[0],qs[1],qs[2]);
		else
			Error("qs must be a vector of size 1,2,3.");
	}
	void updateMomentum(Real qx = 0, Real qy = 0, Real qz = 0)
	{
		args->add("qx",qx);
		args->add("qy",qy);
		args->add("qz",qz);
		_build();
	}

	void read(istream& is)
    {
    	Operator::read(is);
    	itensor::read(is,O);
    }
    
    void write(ostream& os) const
    {
    	Operator::write(os);
    	itensor::write(os,O);
    }

private:
	void _build()
	{
		auto N = args->getInt("N");
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
		_build_O(coeff);
    	
	}

	static Real _sin(Real x)
	{
		return sin(x);
	}
	static Real _cos(Real x)
	{
		return cos(x);
	}

protected:

	virtual void _build_O(Real coeff)
	{
		auto qx = args->getReal("qx");
		auto qy = args->getReal("qy");
		auto qz = args->getReal("qz");
		auto s_op = args->getString("localOperator");
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

};
#endif