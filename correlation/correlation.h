#ifndef __CORRELATION_H_
#define __CORRELATION_H_

#include "service/service.h"
#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class Correlation : public Service
{
protected:

	Evolver* evolver;
	Measurement* measurement;
	State bra;
	Operator* op;

	bool thermal;
	Real tau;

	State ket;
	Real time;
	vector<StringReal> times;
	vector<vector<StringReal>> res;

public:
	Correlation(Args* a, Evolver* e, Measurement* ms) : Service(a), evolver(e), measurement(ms) {}
	Correlation(Args* a, Evolver* e, Measurement* ms, State* s, Operator* o) : Service(a), evolver(e), measurement(ms), op(o)
	{
		bra = *s;
		thermal = args->getBool("thermal"); tau = args->getReal("time-tau");
		time = 0; times.push_back(time);
		ket = op->multiply(bra);
		res.push_back(measurement->measure(bra,ket));
		evolver->setup(BondGate::tReal, tau);
	}

	virtual void calculate()
	{
		if(thermal) evolver->evolve(bra); // Sloppy, would be better to have a bra_evolver and a ket_evolver, even if they are the same.
		else bra.scale(exp(-Complex_i * tau * (bra.getE0())));
		evolver->evolve(ket);
		res.push_back(measurement->measure(bra,ket));
		time += tau; times.push_back(time);
	}
	
	Real getTime() { return time; }

	static string getHash(Args* args)
	{
		vector<string> strings{"SiteSet","Lattice","Model","Evolver"};
		vector<string> reals{"N","MaxDim","beta","beta-tau","time-tau"};
		return "Correlation" + hash_real(reals,args) + hash_string(strings, args);
	}

	virtual void load(ifstream & f)
    {
    	evolver->read(f);
    	read(f,bra);
    	read(f,ket);
    	read(f,time);
    	read(f,times);
    	read(f,res);
    	thermal = args->getBool("thermal"); tau = args->getReal("time-tau");
		evolver->setup(BondGate::tReal, tau);
    }

	virtual void save(ofstream & f)
	{ 
		evolver->write(f);
		write(f,bra);
		write(f,ket);
		write(f,time);
		write(f,times);
		write(f,res);
	}

private:
	virtual vector<string> _labels()
	{
		auto labels = measurement->addLabels();
		labels.push_back("t");
		labels.push_back("MaxDim");
		labels.push_back("N");
		if(args->getString("Lattice") == "Triangular") { labels.push_back("Nx"); labels.push_back("Ny"); }
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("time-tau"); }
		else { labels.push_back("E0"); }
		for(auto& x : evolver->getParams()){ labels.push_back(x.first); }
		labels.push_back("Evolver");
		return labels;
	}

	virtual vector<vector<StringReal>> _results()
	{
		auto results = vector<vector<StringReal>>();
		for(auto i : range(res.size()))
		{
			auto temp = measurement->addResults(res[i]);
			temp.push_back(times[i]);
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			if(args->getString("Lattice") == "Triangular") { temp.push_back(args->getInt("Nx")); temp.push_back(args->getInt("Ny")); }
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("time-tau")); }
			else { temp.push_back(bra.getE0()); }
			for(auto& x : evolver->getParams()){ temp.push_back(x.second); }
			temp.push_back(args->getString("Evolver"));
			results.push_back(temp);
		}
		return results;
	}

};
#endif