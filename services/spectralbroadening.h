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
	vector<Cplx> etas;
    vector<Real> omegas;
	vector<vector<vector<Real>>> preResults;


	/* Outputs */
	vector<string> labels;
	vector<vector<Real>> results;
	Real E0;
	int iterations;

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
		auto thermal = args->getBool("thermal");
		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
		else model = modelBuilder->build(args);
		auto reortho = args->getBool("reorthogonalize");
		if(reortho) krylov = krylovBuilder->build(args,KrylovBuilder::reorthogonalize);
		else krylov = krylovBuilder->build(args,KrylovBuilder::bare);
		auto Ts = krylov->getTs();
		auto N = krylov->getIterations();
		iterations = N;
		E0 = krylov->getE0();
		omegas = getOmegas();
		for(auto i : range(Ts.size())) preResults.push_back(calcBroadening(Ts[i],N));
		processResults();
		repo.save(getHash(),labels,results); // Need a hash.
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
        auto wi = args->getReal("wi");
        auto wf = args->getReal("wf");
        auto n  = args->getInt("nw");
        auto omegas = linspace(wi,wf,n);
        return omegas;
    }
    vector<Cplx> getEtas()
    {
        auto retas = stringToVector(args->getString("etas"));
        for(auto x : retas) etas.push_back(Cplx(0,x));
    }

	void processResults()
	{
		// pre_results in format [t][eta][omega]
		// want it in eta omega S SR
		auto mom = args->defined("qFactor");
		labels.push_back("eta");
		labels.push_back("omega");
		labels.push_back("S");
		labels.push_back("SR");
		labels.push_back("E0");
		if(mom) labels.push_back("qFactor");
		else labels.push_back("position");
		labels.push_back("iterations");
		labels.push_back("maxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");

		for(auto i : range(etas.size()))
		for(auto j : range(omegas.size()))
		{
			results.push_back(etas[i].imag()); // only need imaginary part, real is zero.
			results.push_back(omegas[j]);
			results.push_back(pre_results[0][i][j]);
			if(results.size() == 2) {results.push_back(pre_results[1][i][j]); }
			else {results.push_back(NAN); }
			results.push_back(E0);
			if(mom) {results.push_back(args->getReal("qFactor")); }
			else {results.push_back(args->getReal("position")); }
			results.push_back(iterations);
			results.push_back("iterations");
			results.push_back(args->getReal("maxDim"));
			results.push_back(args->getReal("N"));
			results.push_back(args->getReal("Lattice"));
			results.push_back(args->getReal("Model"));
			results.push_back(Real(args->getBool("thermal")));
		}
	}

	/* */
	virtual string getHash()
	{
		return Krylov::getHash(args) + "_SpectralBroadening";
	}

};

#endif
