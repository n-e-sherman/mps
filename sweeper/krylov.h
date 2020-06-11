#ifndef __KRYLOV_H_
#define __KRYLOV_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "sweeper/sweeper.h"


using namespace std;
using namespace itensor;

class Krylov : public Sweeper
{
protected:
	Matrix Heff;
	std::vector<ITensor> lanczos_vectors;
public:

	Krylov(Args* a) : Sweeper(a) {}
	Krylov(){}
	~Krylov(){}
	virtual void update(LocalMPO const& PH, ITensor& phi)
	{
		auto thresh   = args->getReal("phiThreshold");
		auto nrm = norm(phi);
		if(nrm < std::abs(thresh)) // The entire state was 'high energy'
		{
			return;
		}
	    auto max_iter = args->getInt("MaxIter");
	    auto beta_tol = args->getReal("NormCutoff");
	    
		phi /= nrm;
		ITensor v1 = phi;
	    ITensor v0;
	    ITensor w;
	    v1 /= norm(v1);
	    lanczos_vectors = std::vector<ITensor>({v1});
	    Heff = Matrix(max_iter + 2, max_iter + 2);
	    std::fill(Heff.begin(), Heff.begin()+Heff.size(), 0.);	

	    double beta = 0;
	    for (int iter = 0; iter < max_iter; ++iter)
	    {
	        PH.product(v1, w);
	        double alpha = real(eltC(dag(w) * v1));
	        Heff(iter, iter) = alpha;
	        w -= alpha * v1;
	        if (iter > 0)
	            w -= beta * v0;
	        v0 = v1;
	        beta = norm(w);

	        if (std::abs(beta) < beta_tol)
	            break;

	        v1 = w;
	        v1 /= beta;
	        lanczos_vectors.push_back(v1);
	        Heff(iter+1, iter) = beta;
	        Heff(iter, iter+1) = beta;
	        if(converged())
	        	break;
	    }
	    Heff = subMatrix(Heff,0,lanczos_vectors.size(),0,lanczos_vectors.size());
	    calculate(phi);
	    phi *= nrm;
	    return;
    }

    virtual bool converged()
    {
    	return false; 
    }

    virtual void calculate(ITensor& phi) = 0;

};

#endif