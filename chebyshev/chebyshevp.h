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
	vector<StringReal> res;

public:

	Chebyshevp(Args* a) : Chebyshev(a){}
	Chebyshevp(Args* a, Model* m, Sweeper* swp) : Chebyshev(a,m,swp){}
	Chebyshevp(Args* a, Model* m, State *s, Sweeper* swp, Operator* O) : Chebyshev(a,m,s,swp)
	{ 
		/* Same. */
		psi = state->getState();
		psi.position(1);
		H.position(1);

		/* p specific */
		auto op_q = O->build(args->getReal("qFactor"));
		psi = noPrime(applyMPO(op_q,psi));
		t0 = MPS(psi);
		
		/* Same. */
		t1 = noPrime(applyMPO(H,t0));
		t2 = t1;
		is = siteInds(t0);
		iteration = 1;

		/* p specific */
		res.push_back(innerC(psi,t0));
		res.push_back(innerC(psi,t1));
	}
	
	~Chebyshevp(){}
	virtual void calculate(int iterations)
	{
		for(auto i : range1(iterations))
		{
			cout << iteration + i << endl;
			auto temp = noPrime(applyMPO(H,t1));
			temp *= 2;
			prepare(temp,t0,is);
			t2 = sum(temp,-1*t0,*args);
			// sweeper->sweep(H,t2);
			// if(args->getBool("details")) details.push_back(sweeper->get_details());
			
			t0 = t1;
			t1 = t2;
			res.push_back(innerC(psi,t2));
		}
		iteration += iterations;
	}

	void processResults()
	{
		labels.clear();
		results.clear();
		auto mom = args->defined("qFactor");
		labels.push_back("moment");
		if(args->getBool("imaginary")) labels.push_back("Imoment");
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
		for(auto x : model->getParams()){ labels.push_back(x.first); }
		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(res[i].real());
			if(args->getBool("imaginary")) temp.push_back(res[i].imag());
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
			for(auto x : model->getParams()){ temp.push_back(x.second); }
			results.push_back(temp);
		}
	}

	virtual void load(ifstream& f)
	{
		Chebyshev::load(f);
		read(f,res);
	}

	virtual void save(ofstream& f)
	{
		Chebyshev::save(f);
		write(f,res);
	}
};

#endif