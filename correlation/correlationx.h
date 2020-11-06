#ifndef __CORRELATIONX_H_
#define __CORRELATIONX_H_

#include "correlation/correlation.h"

class Correlationx : public Correlation
{
protected:
	/* Inputs */
	Measurement* measurement;
	vector<vector<StringReal>> res;

public:
	Correlationx(Args* a, Evolver* e, Model* m, Measurement* ms)  : Correlation(a,e,m), measurement(ms) {}
	Correlationx(Args* a, State* s, Evolver* e, Model* m, Measurement* ms)  : Correlation(a,s,e,m), measurement(ms)
	{ 
		auto N = args->getInt("N");
		auto sites = model->getSites();
		auto s_op = args->getString("localOperator");
		int c = ((psi.length() == N) ? N/2 : N-1);
		psiB.position(c);
		auto temp = psiB(c) * op(sites,s_op,c);
		temp.noPrime();
		psiB.set(c,temp);
		psiB.position(1);
		psi.position(1);
		res.push_back(measurement->calculate(psi,psiB));
	}

	void  calculate()
	{
		evolver->evolve(psi);
		evolver->evolve(psiB);
		res.push_back(measurement->calculate(psi,psiB));
		time += tau;
		times.push_back(time);
	}

	void processResults()
	{ 
		labels.clear();
		results.clear();
		for(auto i : range1(args->getInt("N"))) labels.push_back(to_string(i));
		if(args->getBool("imaginary")) for(auto i : range1(args->getInt("N"))) labels.push_back("I" + to_string(i));
		labels.push_back("t");
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
			for(auto j : range(res[i].size())) temp.push_back(res[i][j].real());
			if(args->getBool("imaginary")) for(auto j : range(res[i].size())) temp.push_back(res[i][j].imag());
			temp.push_back(times[i]);
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
    }

	virtual void save(ofstream & f)
	{
		Correlation::save(f);
		write(f,res);
	}
};
#endif