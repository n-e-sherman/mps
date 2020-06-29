#ifndef __PROJECTION_H_
#define __PROJECTION_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "sweeper/sweeper.h"
#include "sweeper/krylov.h"


using namespace std;
using namespace itensor;

class Projection : public Krylov
{
protected:
	Real Ep;
    bool printed = false;
public:

	Projection(Args* a) : Krylov(a) 
	{ 
		Ep = args->getReal("Ep"); 
	}
	Projection(){}
	~Projection(){}

	virtual void calculate(ITensor& phi)
	{
		int dim = nrows(Heff);
        if(dim == 1) return;

        Vector d;
        Matrix U;
        diagHermitian(Heff,U,d);
        vector<int> inds;

        Real max_ = 0;
        for(auto i : range(dim)) 
            max_ = max(max_,abs(d(i)));
        maxE = max(max_,maxE);

        for(auto i : range(dim)) if(std::abs(d(i)) >= Ep) inds.push_back(i); 
        if(inds.size() == 0) // No high energy components
        	return;
        nTrim += inds.size();
        auto Q_phi = ITensor(0*phi);
        for(auto n : range(dim))
        for(auto i : inds)
        {
            Q_phi += U(n,i)*U(0,i)*lanczos_vectors[n];
        }
        phi -= Q_phi;
	    return;
	}

};

#endif