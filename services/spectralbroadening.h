#ifndef __SPECTRALBROADENING_H_
#define __SPECTRALBROADENING_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "services/service.h"
#include "model/modelbuilder.h"
#include "repository/repositorybuilder.h"
#include "krylov/krylovbuilder.h"
#include <string>
#include <iostream>

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
	Real psiiNorm;

	/* Outputs */
	vector<string> labels;
	vector<vector<StringReal>> results;
	Real E0;
	int iterations;

public:
	SpectralBroadening() : Service(){}
	SpectralBroadening(ModelBuilder *mb, KrylovBuilder* kb, RepositoryBuilder* rb) : Service()
	{ 
		modelBuilder = mb;
		repoBuilder = rb;
		krylovBuilder = kb;
	}
	~SpectralBroadening(){}

	void calculate(Args* a)
	{
		cout << "Calculating spectral broadening." << endl;
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
		psiiNorm = krylov->getPsiiNorm();
		etas = getEtas();
		omegas = getOmegas();
		for(auto i : range(Ts.size())) preResults.push_back(calcBroadening(Ts[i],N));
		processResults();
		repo->save(getHash(),"broadening",labels,results);
	}




private:

	vector<vector<Real>> calcBroadening(CMatrix& T, int nMax)
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
		        auto spectralNorm = sqrt(2.0/(Real(args->getInt("N")+1)));
                Print(res);
		        Print(spectralNorm);
		        Print(psiiNorm);
		        Print(M_PI);
                res = -1*(1.0/M_PI)*spectralNorm*psiiNorm*res;
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
        return etas;
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
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");

		for(auto i : range(etas.size()))
		for(auto j : range(omegas.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(etas[i].imag()); // only need imaginary part, real is zero.
			temp.push_back(omegas[j]);
			temp.push_back(preResults[0][i][j]);
			if(preResults.size() == 2) {temp.push_back(preResults[1][i][j]); }
			else {temp.push_back(NAN); }
			temp.push_back(E0);
			if(mom) {temp.push_back(args->getReal("qFactor")); }
			else {temp.push_back(args->getReal("position")); }
			temp.push_back(iterations);
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(Real(args->getBool("thermal")));
			results.push_back(temp);
		}
	}

	/* */
	virtual string getHash()
	{
		return Krylov::getHash(args) + "_SpectralBroadening";
	}

};

#endif
