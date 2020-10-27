#ifndef __CONNECTEDX_H_
#define __CONNECTEDX_H_

#include "measurement/measurement.h"

class Connectedx : public Measurement
{
protected:
	/* Inputs */
	Lattice* lattice;
	SiteSet sites;
	bool thermal;
	vector<int> P;
	vector<int> A;
	vector<StringReal> res;
	

public:

	Connectedx(Args* a, Model* m, Lattice* l)  : Measurement(a, m), lattice(l)
	{ // Used in Chebyshev load.
		sites = model->getSites();
		processSites();
	}
	~Connectedx(){}
	vector<StringReal> calculate(MPS & bra, MPS & psi)
	{
		auto psidag = dag(bra);
		res.clear();
		auto s_op = args->getString("localOperator");
		psidag.replaceSiteInds(siteInds(psi));
    	psidag.replaceLinkInds(sim(linkInds(psidag)));
    	auto L = vector<ITensor>(N+1);
		auto R = vector<ITensor>(N+1);

		L[1] = psi(P[1]) * psidag(P[1]);
		L[1] = L[1] * psi(A[1]) * psidag(A[1]);
		R[N] = psi(A[N]) * psidag(A[N]);
		for(auto i : range1(2,N))
		{	
			auto I = N-i+1;
			L[i] = L[i-1] * psi(P[i]) * psidag(P[i]);
			L[i] = L[i] * psi(A[i]) * psidag(A[i]);
			R[I] = R[I+1] * psi(P[I+1]) * psidag(P[I+1]);
			R[I] = R[I] * psi(A[I]) * psidag(A[I]);
		}
		auto temp = psi(P[1]) * op(sites,s_op,P[1]);
		temp.noPrime();
		temp = temp * R[1];
		temp = temp * psidag(P[1]);
		res.push_back(eltC(temp));
		for(auto i : range1(2,N))
		{
			auto temp = L[i-1]*psidag(P[i]);
			auto temp2 = noPrime(psi(P[i]) * op(sites,s_op,P[i]));
			temp = temp * temp2;
			temp = temp * R[i];
			res.push_back(eltC(temp));
		}
		return res;
	}
	tuple< vector<StringReal>, vector<StringReal> > calculateAll(MPS & bra, MPS & psi)
	{
		auto psidag = dag(bra);
		res.clear();
		auto s_op = args->getString("localOperator");
		psidag.replaceSiteInds(siteInds(psi));
    	psidag.replaceLinkInds(sim(linkInds(psidag)));
		vector<StringReal> resP;
		vector<StringReal> resA;
		auto Nsites = psi.length();
    	auto L = vector<ITensor>(Nsites+1);
		auto R = vector<ITensor>(Nsites+1);

		L[1] = psi(1)*psidag(1);
		R[Nsites] = psi(Nsites)*psidag(Nsites);
		for(auto i : range1(2,Nsites))
		{
			auto I = Nsites-i+1;
			L[i] = L[i-1] * psi(i) * psidag(i);
			R[I] = R[I+1] * psi(I) *psidag(I);
		}

		auto temp = noPrime(psi(1) * op(sites,s_op,1));
		temp = temp*R[2];
		temp = temp * psidag(1);
		resP.push_back(eltC(temp));

		for(auto i : range1(2,Nsites-1))
		{	
			auto temp = L[i-1]*psidag(i);
			auto temp2 = noPrime(psi(i) * op(sites,s_op,i));
			temp = temp * temp2;
			temp = temp * R[i+1];
			if((i % 2) == 0) // ancilla
				resA.push_back(eltC(temp));
			else // physical
				{ resP.push_back(eltC(temp)); }
		}
		temp = noPrime(psi(Nsites) * op(sites,s_op,Nsites));
		temp = temp * L[Nsites - 1];
		temp = temp * psidag(Nsites);
		resA.push_back(eltC(temp));

		return tuple< vector<StringReal>, vector<StringReal> >(resP,resA);
	}

	void processSites()
	{	
		P.push_back(0);
		A.push_back(0);
		for(auto x : lattice->getSites())
		{
			if(x.t == Lattice::physical) 
			{
				P.push_back(x.s);
			}
			else{ A.push_back(x.s); }
		}
	}

	void processResults()
	{
		labels.clear();
		results.clear();

		/* Labels */
		labels.push_back("x");
		labels.push_back("expectation");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		for(auto x : model->getParams()){ labels.push_back(x.first); }

		/* Results */
		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(i+1);
			temp.push_back(res[i]);
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
			for(auto x : model->getParams()){ temp.push_back(x.second); }
			results.push_back(temp);
		}
	}
};
#endif