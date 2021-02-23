#ifndef __CHEBYSHEV_H_
#define __CHEBYSHEV_H_

#include "service/service.h"

using namespace std;
using namespace itensor;

class Chebyshev : public Service
{
protected:

	Measurement* measurement;
	Model* model;
	Operator* op;

	State state;
	State t0;
	State t1;
	State t2;
	int iteration;
	vector<vector<StringReal>> res;

public:

	Chebyshev(Args* a, Measurement* ms, Model* m) : Service(a), measurement(ms), model(m) {}
	Chebyshev(Args* a, Measurement* ms, Model* m, State* s, Operator* o) : Service(a), measurement(ms), model(m), op(o)
	{ 
		state = *s;
		setupModel();
		t0 = op->multiply(state);
		t1 = model->multiply(t0);
		t2 = t1;
		res.push_back(measurement->measure(state,t0));
		res.push_back(measurement->measure(state,t1));
		iteration = 1;
	}
	
	virtual void calculate()
	{
		auto temp = model->multiply(t1);
		temp.scale(2);
		t2.getState() = sum(temp.getState(),-1*(t0.getState()),*args);
		t0 = t1;
		t1 = t2;
		res.push_back(measurement->measure(state,t2));
		iteration++;
	}
	
	virtual int getIteration(){ return iteration; }

	static string getHash(Args* args)
	{	
		vector<string> strings{"SiteSet","Lattice","Model"};
		vector<string> reals{"N","MaxDim","beta","beta-tau","W","Wp"};
		return "Chebyshev" + hash_real(reals, args) + hash_string(strings, args);
	}

	virtual void load(ifstream & f)
	{
		model->read(f);
		read(f,state);
		read(f,t0);
		read(f,t1);
		read(f,t2);
		read(f,iteration);
		read(f,res);
	}

	virtual void save(ofstream & f) const
	{
		model->write(f);
		write(f,state);
		write(f,t0);
		write(f,t1);
		write(f,t2);
		write(f,iteration);
		write(f,res);
	}

private:
	virtual void setupModel()
	{
		auto skip = args->getBool("Skip");
		auto W = args->getReal("W");
		auto Wp = args->getReal("Wp");
		auto scale = (2.0*Wp)/W;
		auto shift = -(scale * state.getE0()) - Wp;
		if (args->getBool("thermal")) { model->calcL(skip, scale, 0); }
		else { model->calcH(skip, scale, shift); }
		auto MPOMaxDim = args->getInt("MPOMaxDim");
		if(maxLinkDim(model->getO()) > MPOMaxDim)
		{
			cout << "compressing MPO from chi = " << maxLinkDim(model->getO()) << "too chi = " << endl;
			auto MaxDim = args->getInt("MaxDim");
			args->add("MaxDim",MPOMaxDim);
			model->orthogonalize();
			cout << maxLinkDim(model->getO()) << endl;
		}
	}

	virtual vector<string> _labels()
	{
		auto labels = measurement->addLabels();
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("beta-tau"); }
		labels.push_back("W");
		labels.push_back("Wp");
		for(auto& x : model->getParams()){ labels.push_back(x.first); }
		return labels;
	}

	virtual vector<vector<StringReal>> _results()
	{
		auto results = vector<vector<StringReal>>();
		for(int i : range(res.size()))
		{
			auto temp = measurement->addResults(res[i]);
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("beta-tau")); }
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			results.push_back(temp);
		}
		return results;
	}

};
#endif