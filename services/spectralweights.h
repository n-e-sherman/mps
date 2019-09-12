#ifndef __SPECTRALWEIGHTS_H_
#define __SPECTRALWEIGHTS_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include "services/service.h"

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
	KrylovBuilder* krylovBuilder;
	Krylov* krylov;

	/* Outputs */

	// TODO: Format is not complete, you need to give everything that specifies this run in labels and results....
	vector<string> labels;
	vector<vector<Real>> results;

public:
	SpectralWeights() : Service(){}
	SpectralWeights(ModelBuilder *mb, RepositoryBuilder* rb, KrylovBuilder* kb) : Service()
	{ 
		modelBuilder = mb;
		repoBuilder = rb;
		krylovBuilder = kb;
	}
	~SpectralWeights(){}

	void calculate(Args* a)
	{
		args = a;
		repo = repoBuilder->build(args);
		auto thermal = args->getYesNo("thermal",0);
		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
		else model = modelBuilder->build(args);
		krylov = krylovBuilder->build(args,KrylovBuilder::reorthogonalize);
		E0 = krylov->getE0();
		auto M = krylov->getMatrices();
		auto N = krylov->getIterations();
		calcWeights(M,N);
		repo.save(results); // Need a hash.
	}

private:

	void calcWeights(Krylov::Matrices* M, N)
	{
		Vector d;
        CMatrix U;
        auto residues = Matrix(N,N);
        for(auto &el : residues) el = NAN;
        auto weights = Matrix(N,N);
        for(auto &el : weights) el = NAN;
        auto frequencies = Matrix(N,N);
        for(auto &el : frequecies) el = NAN;
        auto frequenciesBare = Matrix(N,N);
        for(auto &el : frequenciesBare) el = NAN;
       	for(n : range1(N))
       	{
	        auto Tref = subMatrix(M->tr,0,n,0,n);
	        diagHermitian(Tref,U,d);
	        Vector dBare;
	        CMatrix UBare;
	        auto TrefBare = subMatrix(M->t,0,n,0,n);
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
	        for(auto k : range(n)) // may be n+1
	        {
	            auto tempH = Cplx(0,0);
	            auto tempH2 = Cplx(0,0);
	            for(auto i : range(n))
	            for(auto j : range(n))
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
		labels.push_back("iterations");
		labels.push_back("E0");
		for(auto i : range1(n)) labels.push_back("Barefrequencies"+to_string(i));
		for(auto i : range1(n)) labels.push_back("frequencies"+to_string(i));
		for(auto i : range1(n)) labels.push_back("residues"+to_string(i));
		for(i : range1(n)) labels.push_back("weights"+to_string(i));
		auto temp = vector<Real>;
		for(auto i : range(n))
		{
			temp.push_back(i+1);
			temp.push_back(E0);
			for(auto j : range(n)) temp.push_back(frequenciesBare(i,j));
			for(auto j : range(n)) temp.push_back(frequencies(i,j));
			for(auto j : range(n)) temp.push_back(residues(i,j));
			for(auto j : range(n)) temp.push_back(weights(i,j));
			results.push_back(temp);
		}
	}


};

#endif