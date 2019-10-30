#ifndef __SPECTRALWEIGHTS_H_
#define __SPECTRALWEIGHTS_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "services/service.h"
#include "model/modelbuilder.h"
#include "repository/repositorybuilder.h"
#include "lanczos/lanczosbuilder.h"
#include <string>
#include <iostream>

using namespace itensor;
using namespace std;

class SpectralWeights : public Service
{
private:
	/* Inputs */
	Args* args;
	ModelBuilder* modelBuilder;
	Model* model;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
	LanczosBuilder* lanczosBuilder;
	Lanczos* lanczos;

	/* Helpers */
	Real E0;

	/* Outputs */
	vector<string> labels;
	vector<vector<StringReal>> results;

public:
	SpectralWeights() : Service(){}
	SpectralWeights(ModelBuilder *mb, LanczosBuilder* lb, RepositoryBuilder* rb) : Service()
	{ 
		modelBuilder = mb;
		repoBuilder = rb;
		lanczosBuilder = lb;
	}
	~SpectralWeights(){}

	void calculate(Args* a)
	{
		cout << "Calculating spectral weights." << endl;
		args = a;
		repo = repoBuilder->build(args);
		auto thermal = args->getBool("thermal");
		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
		else model = modelBuilder->build(args);
		lanczos = lanczosBuilder->build(args,LanczosBuilder::reorthogonalize);
		E0 = lanczos->getE0();
		auto M = lanczos->getMatrices();
		auto N = lanczos->getIterations();
		calcWeights(M,N);
		repo->save(getHash(),"weights",labels,results); // Need a hash.
	}






private:

	void calcWeights(Lanczos::Matrices* M, int N)
	{
		Vector d;
        CMatrix U;
        auto residues = Matrix(N,N);
        for(auto &el : residues) el = NAN;
        auto weights = Matrix(N,N);
        for(auto &el : weights) el = NAN;
        auto frequencies = Matrix(N,N);
        for(auto &el : frequencies) el = NAN;
        auto frequenciesBare = Matrix(N,N);
        for(auto &el : frequenciesBare) el = NAN;
       	for(auto n : range(N))
       	{
	        auto Tref = subMatrix(M->tr,0,n+1,0,n+1);
	        diagHermitian(Tref,U,d);
	        Vector dBare;
	        CMatrix UBare;
	        auto TrefBare = subMatrix(M->t,0,n+1,0,n+1);
	        diagHermitian(TrefBare,UBare,dBare);

	        auto temp = vector<Real>();
	        for(auto el : row(U,0)) temp.push_back(abs(el)*abs(el)); // Maybe column?
	        for(auto i : range(temp.size())) weights(n,i) = temp[i];

	        temp = vector<Real>();
	        for(auto el : d) temp.push_back(el);
	        for(auto i : range(temp.size())) frequencies(n,i) = temp[i];

	        temp = vector<Real>();
	        for(auto el : dBare) temp.push_back(el);
	        for(auto i : range(temp.size())) frequenciesBare(n,i) = temp[i];

	        temp = vector<Real>();
	        for(auto k : range(n+1)) // may be n+1
	        {
	            auto tempH = Cplx(0,0);
	            auto tempH2 = Cplx(0,0);
	            for(auto i : range(n+1))
	            for(auto j : range(n+1))
	            for(auto p : range(i+1))
	            for(auto q : range(j+1))
	            {
	                tempH +=  U(i,k)*U(j,k)*(M->s(p,i))*(M->s(q,j))*(M->h(p,q));
	                tempH2 += U(i,k)*U(j,k)*(M->s(p,i))*(M->s(q,j))*(M->h2(p,q));
	            }
	            temp.push_back( abs(tempH2 + d[k]*d[k] - (2*d[k]*tempH)) );
	        }
	        for(auto i : range(temp.size())) residues(n,i) = temp[i];
	    }
		auto mom = args->defined("qFactor");
		labels.push_back("iterations");
		for(auto i : range1(N)) labels.push_back("frequenciesBare"+to_string(i));
		for(auto i : range1(N)) labels.push_back("frequencies"+to_string(i));
		for(auto i : range1(N)) labels.push_back("residues"+to_string(i));
		for(auto i : range1(N)) labels.push_back("weights"+to_string(i));
		labels.push_back("E0");
		if(mom) labels.push_back("qFactor");
		else labels.push_back("position");
		labels.push_back("nLanczos");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		labels.push_back("Method");
		if(args->getString("Method")=="Fit") labels.push_back("Nsweep");
		if(args->getBool("squared")) labels.push_back("squared");

		for(auto i : range(N))
		{
			auto temp = vector<StringReal>();
			temp.push_back(i+1);
			for(auto j : range(N)) temp.push_back(frequenciesBare(i,j));
			for(auto j : range(N)) temp.push_back(frequencies(i,j));
			for(auto j : range(N)) temp.push_back(residues(i,j));
			for(auto j : range(N)) temp.push_back(weights(i,j));
			temp.push_back(E0);
			if(mom) temp.push_back(args->getReal("qFactor"));
			else temp.push_back(args->getReal("position"));
			temp.push_back(args->getReal("nLanczos"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model")); 
			temp.push_back(Real(args->getBool("thermal")));
			temp.push_back(args->getString("Method"));
			if(args->getString("Method")=="Fit") temp.push_back(args->getReal("Nsweep"));
			if(args->getBool("squared")) temp.push_back(1);
			results.push_back(temp);
		}
	}
	
	virtual string getHash()
	{
		return Lanczos::getHash(args) + "_SpectralWeights";
	}


};

#endif
