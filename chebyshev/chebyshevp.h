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
			t1 = noPrime(applyMPO(H,t0,*args));
			t2 = t1;
			res.push_back(inner(t0,t1));
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
		labels.push_back("qFactor");
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		labels.push_back("W");
		labels.push_back("Wp");
		for(auto& x : model->getParams()){ labels.push_back(x.first); }

		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(res[i]);
			temp.push_back(args->getReal("qFactor"));
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			results.push_back(temp);
		}
	}

	virtual void load(ifstream & f)
	{
		Chebyshev::load(f);
		read(f,res);
		read(f,m0);
	}

	virtual void save(ofstream & f)
	{
		Chebyshev::save(f);
		write(f,res);
		write(f,m0);
	}
};

#endif