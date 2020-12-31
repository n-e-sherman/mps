#ifndef __LOCAL_H_
#define __LOCAL_H_

#include "sites/sitesbuilder.h"
#include "measurement/measurement.h"

class Local : public Measurement
{
protected:
	/* Inputs */
	Lattice* lattice;
	Sites* sites;	

public:

	Local(Args* a, Sites* s)  : Measurement(a), sites(s) {}

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

		/* Build Environment */
		L[1] = psi(1) * psidag(1);
		R[N] = psi(N) * psidag(N);
		for(auto i : range1(2,N))
		{	
			auto I = N-i+1;
			L[i] = L[i-1] * psi(i) * psidag(i);
			R[I] = R[I+1] * psi(I) * psidag(I);
		}

		/* Calculate O(x) */
		auto temp = psi(1) * op(sites->getSites(),s_op,1);
		temp.noPrime();
		temp = temp * R[2];
		temp = temp * psidag(1);
		res.push_back(eltC(temp));
		for(auto i : range1(2,N-1))
		{
			auto temp = L[i-1]*psidag(i);
			auto temp2 = noPrime(psi(i) * op(sites->getSites(),s_op,i));
			temp = temp * temp2;
			temp = temp * R[i+1];
			res.push_back(eltC(temp));
		}
		temp = L[N-1]*psidag(N);
		auto temp2 = noPrime(psi(N) * op(sites->getSites(),s_op,N));
		temp = temp * temp2;
		res.push_back(eltC(temp));
		return res;
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

