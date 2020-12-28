#ifndef __REALSPACE_H_
#define __REALSPACE_H_

#include "sites/sitesbuilder.h"
#include "measurement/measurement.h"
#include "lattice/lattice.h"

class RealSpace : public Measurement
{
protected:
	/* Inputs */
	Lattice* lattice;
	Sites* sites;

	/* Helpers */
	vector<int> P;
	vector<int> A;
	
	

public:

	RealSpace(Args* a, Lattice* l, Sites* s)  : Measurement(a), lattice(l), sites(s) { processSites(); }

	virtual vector<StringReal> measure(State& bra, State& psi) // make const?
	{
		vector<StringReal> res;
		auto s_op = args->getString("localOperator");
		auto N = args->getInt("N");
		auto psidag = bra.dag();
		psidag.replaceSiteInds(psi.siteInds());
    	psidag.replaceLinkInds(sim(psidag.linkInds()));
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
		auto temp = psi(P[1]) * op(sites->getSites(),s_op,P[1]);
		temp.noPrime();
		temp = temp * R[1];
		temp = temp * psidag(P[1]);
		res.push_back(eltC(temp));
		for(auto i : range1(2,N))
		{
			auto temp = L[i-1]*psidag(P[i]);
			auto temp2 = noPrime(psi(P[i]) * op(sites->getSites(),s_op,P[i]));
			temp = temp * temp2;
			temp = temp * R[i];
			res.push_back(eltC(temp));
		}
		return res;
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

	// Merge Correlation and Chebyshev
	virtual vector<string> addLabels()
	{
		auto res = vector<string>();
		for(auto i : range1(args->getInt("N"))) res.push_back(to_string(i));
		if(args->getBool("imaginary")) for(auto i : range1(args->getInt("N"))) res.push_back("I" + to_string(i));
		return res;
	}
	virtual vector<StringReal> addResults(vector<StringReal> &_in) 
	{
		auto res = vector<StringReal>();
		for(auto j : range(_in.size())) res.push_back(_in[j].real());
		if(args->getBool("imaginary")) for(auto j : range(_in.size())) res.push_back(_in[j].imag());
		return res;
	}

};
#endif

