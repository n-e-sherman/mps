#ifndef __CORRELATIONP_H_
#define __CORRELATIONP_H_

#include "correlation/correlation.h"

class Correlationp : public Correlation
{
protected:
	/* Inputs */
	MPO op_q;
	vector<StringReal> res;

public:
	Correlationp(Args* a, Evolver* e, Model* m)  : Correlation(a,e,m) {}
	Correlationp(Args* a, State* s, Evolver* e, Model* m, Operator* o)  : Correlation(a,s,e,m)
	{ 
		op_q = o->build(args->getReal("qFactor"));
		psiB = noPrime(applyMPO(op_q,psiB));
		psiB.position(1);
		psi.position(1);
		auto psidag = dag(psiB);
		res.push_back(innerC(psi,op_q,psiB));
	}

	void  calculate()
	{
		evolver->evolve(psi);
		evolver->evolve(psiB);
		res.push_back(innerC(psi,op_q,psiB));
		time += tau;
		times.push_back(time);
	}

	void processResults()
	{ 
		labels.clear();
		results.clear();
		labels.push_back("c");
		if(args->getBool("imaginary")) labels.push_back("Ic");
		labels.push_back("t");
		labels.push_back("qFactor");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		for(auto& x : model->getParams()){ labels.push_back(x.first); }

		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(res[i].real());
			if(args->getBool("imaginary")) temp.push_back(res[i].imag());
			temp.push_back(times[i]);
			temp.push_back(args->getReal("qFactor"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			results.push_back(temp);
		}
	}

	virtual void load(ifstream & f)
    {
        Correlation::load(f);
        read(f,res);
        read(f,op_q);
    }

	virtual void save(ofstream & f)
	{
		Correlation::save(f);
		write(f,res);
		write(f,op_q);
	}
};
#endif