#ifndef __EXACT_H_
#define __EXACT_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "sweeper/sweeper.h"


using namespace std;
using namespace itensor;

class Exact : public Sweeper
{
protected:
    Real Ep;
public:

	Exact(Args* a) : Sweeper(a) 
    { 
        Ep = args->getReal("Ep"); 
    }
	Exact(){}
	~Exact(){}
	virtual void update(LocalMPO const& PH, ITensor& phi)
	{
		auto L = PH.L();
		auto R = PH.R();
		ITensor Heff = PH.H()(b);
		if(numCenter == 2)
			Heff *= PH.H()(b+1);
		if(L.order() > 0)
			Heff *= L;
		if(R.order() >0)
			Heff *= R;
        ITensor d,U;
        diagHermitian(Heff,U,d);
        auto func = [=](Real x){ return (std::abs(x) > Ep ? 1.0 : 0.0); }; 
        d.apply(func);
        auto d_norm = norm(d);
        if(d_norm == 0) // No high energy components
            return;
        nTrim += d_norm;
        auto Q = (U*d)*prime(U);
        auto Q_phi = noPrime(Q*phi);
        phi -= Q_phi;
	    return;

    }
};

#endif