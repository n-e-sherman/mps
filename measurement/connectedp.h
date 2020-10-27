#ifndef __CONNECTEDP_H_
#define __CONNECTEDP_H_

#include "measurement/measurement.h"

class Connectedp : public Measurement
{
protected:
	/* Inputs */
	Lattice* lattice;
	SiteSet sites;
	bool thermal;
	Operator* O;

	/* Outputs */
	vector<Real> qFactors;
	vector<StringReal> res;

public:

	Connectedp(Args* a, Model* m, Operator* o)  : Measurement(a, m), O(o) {}
	vector<StringReal> calculate(MPS & bra, MPS & ket)
	{
		res.clear();
		qFactors.clear();
		auto s_op = args->getString("localOperator");
		for(Real i : range1(N))
		{
			auto qFactor = i/Real(N);
			qFactors.push_back(qFactor);
			res.push_back(innerC(bra,O->build(qFactor),ket));
		}
		return res;
	}
	tuple< vector<StringReal>, vector<StringReal> > calculateAll(MPS & bra, MPS & ket)
	{ 
		auto res = calculate(bra,ket);
		return tuple< vector<StringReal>, vector<StringReal> >(res,res);
	}
	

	void processResults()
	{
		labels.clear();
		results.clear();

		/* Labels */
		labels.push_back("qFactor");
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
			temp.push_back(qFactors[i]);
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