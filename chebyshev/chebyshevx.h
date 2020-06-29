#ifndef __CHEBYSHEVX_H_
#define __CHEBYSHEVX_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "chebyshev/chebyshev.h"


using namespace std;
using namespace itensor;

class Chebyshevx : public Chebyshev
{
protected:
	Lattice* lattice;
	SiteSet sites;
	MPS psidag;
	vector<vector<Real>> res; // [iteration][site], site starts 0 NOT 1
	vector<int> P;
	vector<int> A;
	int N;
	vector<Real> errorMPO;

public:

	Chebyshevx(Args* a) : Chebyshev(a){}
<<<<<<< HEAD
	Chebyshevx(Args* a, Model* m, State *s, Lattice* l) : Chebyshev(a,m,s)
=======
	Chebyshevx(Args* a, Model* m, Sweeper* swp) : Chebyshev(a,m,swp){}
	Chebyshevx(Args* a, Model* m, State *s, Lattice* l, Sweeper* swp) : Chebyshev(a,m,s,swp)
>>>>>>> temp-branch
	{
		if(args->getBool("errorMPOProd")) errorMPO = vector<Real>({0,0});
		lattice = l;
		processSites();
		sites = model->getSites();	
		psi = state->getState();
		psi.position(1);
		H.position(1);
		t0 = MPS(psi);
		psidag = dag(psi);
		N = args->getInt("N");
<<<<<<< HEAD
		auto c = N/2;

=======
		int c = N/2;
>>>>>>> temp-branch
		t0.position(P[c]);
		auto temp = noPrime(t0(P[c]) * op(sites,"Sz",P[c]));
		t0.set(P[c],temp);
		t0.position(1);
		res.push_back(calcMoments(t0));
		t1 = noPrime(applyMPO(H,t0,*args));
		t2 = t1;
		res.push_back(calcMoments(t1));
		H.position(1);
		is = siteInds(t0);
		iteration = 1;
	}
	
	~Chebyshevx(){}
	virtual void calculate(int iterations)
	{
		for(auto i : range1(iterations))
		{
			cout << iteration + i << endl;
			auto temp = noPrime(applyMPO(H,t1,*args));
			if(args->getBool("errorMPOProd")) errorMPO.push_back(errorMPOProd(temp,H,t1));
			temp *= 2;
			prepare(temp,t0,is);
			t2 = sum(temp,-1*t0,*args);
			sweeper->sweep(H,t2);
			if(args->getBool("details")) details.push_back(sweeper->get_details());
			res.push_back(calcMoments(t2));
			t0 = t1;
			t1 = t2;
		}
		iteration += iterations;
	}

	vector<Real> calcMoments(MPS const &in)
	{
		/******************************************
		 * I AM ASSUMING THE MOMENTS WILL BE REAL *
		 ******************************************/

		psidag.replaceSiteInds(siteInds(in));
    	psidag.replaceLinkInds(sim(linkInds(psidag)));

		auto L = vector<ITensor>(N+1);
		auto R = vector<ITensor>(N+1);
		auto result = vector<Real>(N+1);
		L[1] = ( psidag(P[1])*in(P[1]) ) * ( psidag(A[1]) * in(A[1]) );
		R[N] = psidag(A[N])*in(A[N]);
		for(auto i : range1(2,N))
		{
			auto I = N-i+1;
			L[i] = L[i-1] * ( ( psidag(P[i])*in(P[i]) ) * ( psidag(A[i]) *in(A[i]) ) );
			R[I] = R[I+1] * ( ( psidag(P[I+1])*in(P[I+1]) ) * ( psidag(A[I]) *in(A[I]) ) );
		}
		auto temp = psidag(P[1]) * noPrime(in(P[1]) * op(sites,"Sz",P[1])) * R[1];
		result.push_back(elt(temp));
		for(auto i : range1(2,N))
		{
			auto temp = L[i-1] * psidag(P[i]) * noPrime(in(P[i]) * op(sites,"Sz",P[i])) * R[i];
			result.push_back(elt(temp));
		}
		return result;
	}

	void processSites()
	{	
		P.push_back(0);
		A.push_back(0);
		for(auto x : lattice->getSites())
		{
			if(x.t == Lattice::physical) P.push_back(x.s);
			else{ A.push_back(x.s); }
		}
	}

	void processResults()
	{
		/* res in formalt [moment], want to add settings to labels and results */
		labels.clear();
		results.clear();
		for(auto i : range1(args->getInt("N"))) labels.push_back(to_string(i));
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		labels.push_back("W");
		labels.push_back("Wp");
		labels.push_back("sweeperType");
		labels.push_back("sweeperCount");
		labels.push_back("MaxIter");
		for(auto& x : model->getParams()){ labels.push_back(x.first); }
		for(auto& x : detail_labels){ labels.push_back(x); }
		if(args->getBool("errorMPOProd")){ labels.push_back("errorMPOProd"); }

		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			for(auto j : range(res[i].size())) temp.push_back(res[i][j]);
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			string sProj = args->getString("sweeperType");
			temp.push_back(sProj);
			if(sProj == "identity"){ temp.push_back(0.0); temp.push_back(0.0); }
			else
			if(sProj == "exact"){ temp.push_back(args->getReal("sweeperCount")); temp.push_back(0.0); }
			else
			if(sProj == "projection"){ temp.push_back(args->getReal("sweeperCount")); temp.push_back(args->getReal("MaxIter")); }
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			for(auto& x : details[i]){temp.push_back(x); }
			if(args->getBool("errorMPOProd")) temp.push_back(errorMPO[i]);
			results.push_back(temp);
		}
	}
	virtual void load(ifstream & f)
	{
		Chebyshev::load(f);
		read(f,res);
		read(f,psidag);
		read(f,N);
		read(f,P);
		read(f,A);
		read(f,errorMPO);
		psidag.position(1);
		auto sType = args->getString("SiteSet");
		if     (sType == "SpinHalf"){ auto rSites = SpinHalf(args->getInt("N")); rSites.read(f); sites = rSites; }
		else if(sType == "SpinOne") { auto rSites = SpinOne(args->getInt("N")); rSites.read(f); sites = rSites;  }
		else if(sType == "SpinTwo") { auto rSites = SpinTwo(args->getInt("N")); rSites.read(f); sites = rSites;  } 	
	}

	virtual void save(ofstream & f)
	{
		Chebyshev::save(f);
		write(f,res);
		write(f,psidag);
		write(f,N);
		write(f,P);
		write(f,A);
		write(f,errorMPO);
		write(f,sites);
	}

};

#endif