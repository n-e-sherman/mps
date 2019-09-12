#ifndef __SPECTRALBROADENING_H_
#define __SPECTRALBROADENING_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include "services/service.h"

using namespace itensor;
using namespace std;

class SpectralBroadening : public Service
{
private:
	/* Inputs */
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	KrylovBuilder* krylovBuilder;
	Krylov* krylov;

	/* Helpers */
	Real E0;
	vector<Cplx> etas;
    vector<Real> omegas;
	vector<vector<vector<Real>>> preResults;


	/* Outputs */
	vector<string> labels;
	vector<vector<Real>> results;

public:
	SpectralBroadening() : Service(){}
	SpectralBroadening(ModelBuilder *mb, RepositoryBuilder* rb, KrylovBuilder* kb) : Service()
	{ 
		modelBuilder = mb;
		repoBuilder = rb;
		krylovBuilder = kb;
	}
	~SpectralBroadening(){}

	void calculate(Args* a)
	{
		args = a;
		repo = repoBuilder->build(args);
		auto thermal = args->getYesNo("thermal",0);
		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
		else model = modelBuilder->build(args);
		auto reortho = args->getYesNo("reortho",0);
		if(reortho) krylov = krylovBuilder->build(args,KrylovBuilder::reorthogonalize);
		else krylov = krylovBuilder->build(args,KrylovBuilder::bare);
		auto Ts = krylov->getTs();
		auto N = krylov->getIterations();
		E0 = krylov->getE0();
		omegas = getOmegas();
		for(auto i : range(Ts.size())) preResults.push_back(calcBroadening(Ts[i],N));
		processResults();
		repo.save(results); // Need a hash.
	}




private:

	vector<vector<Real>> calcBroadening(CMatrix& T, int Nmax)
	{
        Vector d;
        CMatrix U;
        diagHermitian(T,U,d);
        vector<vector<Real>> result;
        for(auto eta : etas)
        {
            vector<Real> temp;
            for(auto omega : omegas)
            {
                auto z = omega + E0 + eta;
                auto delta = CVector(nMax);
                for(int i = 0; i < nMax; i++) delta(i) = 1.0/(z-d(i));
                auto D = CMatrix(nMax,nMax);
                diagonal(D) &= delta;
                auto R = U*D*conj(transpose(U)); // 1/(z-M)
                auto res = R(0,0).imag();
                res = -1*(1.0/M_PI)*psiiNorm*psiiNorm*res;
                temp.push_back(res);
            }
            result.push_back(temp);
        }
        return result;
	}

	vector<Real> getOmegas()
    {
        auto wi = args->getReal("wi",0);
        auto wf = args->getReal("wf",3);
        auto n  = args->getInt("nw",31);
        auto omegas = linspace(wi,wf,n);
        return omegas;
    }
    vector<Cplx> getEtas()
    {
    	auto temp = input.getString("etas","");
        auto retas = stringToVector(temp);
        for(auto x : retas) etas.push_back(Cplx(0,x));
    }

	void processResults()
	{
		// pre_results in format [t][eta][omega]
		// want it in eta omega S SR
	}

};

#endif
