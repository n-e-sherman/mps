#ifndef __CHEBYSHEVP_H_
#define __CHEBYSHEVP_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "chebyshev/chebyshev.h"


using namespace std;
using namespace itensor;

class Chebyshevp : public Chebyshev
{
protected:
	vector<Real> res;
	vector<Real> res2;
	Real m0;

public:

	Chebyshevp(Args* a) : Chebyshev(a){}
	Chebyshevp(Args* a, Model* m, State *s) : Chebyshev(a,m,s)
	{ 
			 /* May want to make this general, might have another thing in mind for the initial state. */
			psi = state->getState();
			psi.position(1);
			t0 = MPS(psi);
			m0 = inner(t0,t0);
			res.push_back(m0); // May want to change inner to innerC if you get complex values at some point. 
			res2.push_back(m0);
			t1 = noPrime(applyMPO(H,t0,*args));
			t2 = t1;
			res.push_back(inner(t0,t1));
			auto temp2 = inner(t1,t1);
			res2.push_back(2*temp2 - m0);
			H.position(1);
			is = siteInds(t0);
			iteration = 1;
	}
	
	~Chebyshevp(){}
	virtual void calculate(int iterations)
	{
		for(auto i : range1(iterations))
		{
			cout << iteration + i << endl;
			auto temp = applyMPO(H,t1,*args);
			temp *= 2;
			prepare(temp,t0,is);
			t2 = sum(temp,-1*t0,*args);
			res.push_back(inner(psi,t2));
			auto temp2 = inner(t2,t2);
			res2.push_back((2.0*temp2) - m0);
			t0 = t1;
			t1 = t2;
		}
		iteration += iterations;
	}

	void processResults()
	{
		/* res in formalt [moment], want to add settings to labels and results */
		labels.clear();
		results.clear();
		auto mom = args->defined("qFactor");
		labels.push_back("moment");
		labels.push_back("moment2");
		labels.push_back("qFactor");
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		labels.push_back("W");
		labels.push_back("Wp");
		labels.push_back("Method");
		if(args->getString("Method")=="Fit") labels.push_back("Nsweep");
		labels.push_back("squared");

		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(res[i]);
			if(i < res2.size()) temp.push_back(res2[i]);
			else{temp.push_back(NAN); }
			temp.push_back(args->getReal("qFactor"));
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			temp.push_back(args->getString("Method"));
			if(args->getString("Method")=="Fit") temp.push_back(args->getReal("Nsweep"));
			temp.push_back(args->getBool("squared"));
			results.push_back(temp);
		}
	}

	virtual void load(ifstream & f)
	{
		Chebyshev::load(f);
		read(f,res);
		read(f,res2);
		read(f,m0);
	}

	virtual void save(ofstream & f)
	{
		Chebyshev::save(f);
		write(f,res);
		write(f,res2);
		write(f,m0);
	}
};

#endif