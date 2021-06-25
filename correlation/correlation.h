#ifndef __CORRELATION_H_
#define __CORRELATION_H_

#include "service/service.h"
#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class Correlation : public Service
{
protected:

	Evolver* bra_evolver;
	Evolver* ket_evolver;
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
	Correlation(Args* a, Evolver* eb, Evolver* ek, Measurement* ms) : Service(a), bra_evolver(eb), ket_evolver(ek), measurement(ms) {}
	Correlation(Args* a, Evolver* eb, Evolver* ek, Measurement* ms, State* s, Operator* o) : Service(a), bra_evolver(eb), ket_evolver(ek), measurement(ms), op(o)
	{
		bra = *s;
		thermal = args->getBool("thermal"); tau = args->getReal("time-tau");
		time = 0; times.push_back(time);
		ket = op->multiply(bra);
		res.push_back(measurement->measure(bra,ket));
		_setup_evolvers();
	}

	virtual void calculate()
	{
		bra_evolver->evolve(bra);
		ket_evolver->evolve(ket);
		res.push_back(measurement->measure(bra,ket));
		time += tau; times.push_back(time);
	}
	
	Real getTime() { return time; }

	static string getHash(Args* args)
	{
		vector<string> strings{"SiteSet","Lattice","Model","Evolver"};
		vector<string> reals{"N","MaxDim","beta","beta-tau","time-tau"};
		auto res = "Correlation" + hash_real(reals,args) + hash_string(strings, args);
		if(!(args->getBool("realTime"))) res += "i";
		return res;
	}

	virtual void load(ifstream & f)
    {
    	bra_evolver->read(f);
    	ket_evolver->read(f);
    	read(f,bra);
    	read(f,ket);
    	read(f,time);
    	read(f,times);
    	read(f,res);
    	thermal = args->getBool("thermal"); tau = args->getReal("time-tau");
		_setup_evolvers();
    }

	virtual void save(ofstream & f)
	{ 
		bra_evolver->write(f);
		ket_evolver->write(f);
		write(f,bra);
		write(f,ket);
		write(f,time);
		write(f,times);
		write(f,res);
	}

private:

	virtual void _setup_evolvers()
	{
		if(args->getBool("realTime"))
		{
			bra_evolver->setup(BondGate::tReal, tau);
			ket_evolver->setup(BondGate::tReal, tau);
		}
		else
		{
			bra_evolver->setup(BondGate::tImag, -tau);
			ket_evolver->setup(BondGate::tImag, tau);	
		}
	}

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
		for(auto& x : ket_evolver->getParams()){ labels.push_back(x.first); }
		labels.push_back("Evolver");
		labels.push_back("Geometry");
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
			for(auto& x : ket_evolver->getParams()){ temp.push_back(x.second); }
			temp.push_back(args->getString("Evolver"));
			temp.push_back(args->getString("Geometry"));
			results.push_back(temp);
		}
		return results;
	}

};
#endif