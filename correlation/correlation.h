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
	Real Mz = -1E-16;
	Real Mx = -1E-16;

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
		_calculate_magnetization();
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
		vector<string> reals{"N","MaxDim","beta","beta-tau","time-tau","M"};
		if(args->getBool("momentum"))
		{
			reals.push_back("qx");
			reals.push_back("qy");
			reals.push_back("qz");
		}
		auto res = "Correlation" + hash_real(reals,args) + hash_string(strings, args);
		if(!(args->getBool("realTime"))) res += "i";
		if(args->getBool("CenterSite")) res += "-CenterSite";
		if(args->getBool("Connected")) res += "-Connected";
		if(args->getBool("tune")) res += "-tune";
		if(args->getBool("disentangle")) res += "-disentangle";
		if(!args->getBool("ConserveQNs")) res += "_noQNs";
		if(!args->getBool("ConserveSz")) res += "_noSz";
		if(args->getString("initial") == "Random") res += "_Random";
		return res;
	}

	virtual void load(ifstream & f)
    {
    	bra_evolver->read(f);
    	ket_evolver->read(f);
    	measurement->read(f);
    	read(f,bra);
    	read(f,ket);
    	read(f,time);
    	read(f,times);
    	read(f,res);
    	_calculate_magnetization();
    	thermal = args->getBool("thermal"); tau = args->getReal("time-tau");
		_setup_evolvers();
    }

	virtual void save(ofstream & f)
	{ 
		bra_evolver->write(f);
		ket_evolver->write(f);
		measurement->write(f);
		write(f,bra);
		write(f,ket);
		write(f,time);
		write(f,times);
		write(f,res);
		write(f,Mz);
		write(f,Mx);
	}

private:

	virtual void _setup_evolvers()
	{
		auto op = "H";
		if ((args->getBool("thermal")) & (args->getBool("disentangle")))op = "L";
		cout << "Operator used for evolver = " << op << endl;
		if(args->getBool("realTime"))
		{
			bra_evolver->setup(BondGate::tReal, tau, op);
			ket_evolver->setup(BondGate::tReal, tau, op);
		}
		else
		{
			bra_evolver->setup(BondGate::tImag, -tau, op);
			ket_evolver->setup(BondGate::tImag, tau, op);	
		}
	}

	virtual void _calculate_magnetization()
	{
		cout << "correlation._calculate_magnetization: calculating" << endl;
		if (
			(args->getString("SiteSet") != "SpinHalf") || 
			(Mz != -1E-16) || 
			(Mx != -1E-16)
			) { cout << "skip computing Mz and Mx" << endl; return;}

		Args* measurement_args = measurement->getArgs();
		auto s_op = measurement_args->getString("localOperator");
		auto N = args->getInt("N");
		measurement->updateArgs("localOperator", "Sz");
		auto Szs = measurement->measure(bra, bra);
		measurement->updateArgs("localOperator", s_op);
		Mz = 0;
		for(auto& Sz : Szs){ Mz += Sz.real(); }

		if (args->getBool("ConserveSz")) return;
		measurement->updateArgs("localOperator", "Sx");
		auto Sxs = measurement->measure(bra, bra);
		measurement->updateArgs("localOperator", s_op);
		Mx = 0;
		for(auto& Sx : Sxs){ Mx += Sx.real(); }



	}

	virtual vector<string> _labels()
	{
		auto labels = measurement->addLabels();
		labels.push_back("t");
		labels.push_back("MaxDim");
		labels.push_back("N");
		if((args->getString("Lattice") == "Triangular") || (args->getString("Lattice") == "Square")) { labels.push_back("Nx"); labels.push_back("Ny"); }
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("time-tau"); }
		else { labels.push_back("E0"); }
		labels.push_back("M");
		labels.push_back("Mz");
		labels.push_back("Mx");
		for(auto& x : ket_evolver->getParams()){ labels.push_back(x.first); }

		labels.push_back("Evolver");
		labels.push_back("Geometry");
		labels.push_back("CenterSite");
		labels.push_back("Connected");
		labels.push_back("tune");
		labels.push_back("disentangle");
		labels.push_back("ConserveQNs");
		labels.push_back("ConserveSz");
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
			if((args->getString("Lattice") == "Triangular") || (args->getString("Lattice") == "Square")) { temp.push_back(args->getInt("Nx")); temp.push_back(args->getInt("Ny")); }
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("time-tau")); }
			else { temp.push_back(bra.getE0()); }
			temp.push_back(args->getReal("M"));
			temp.push_back(Mz);
			temp.push_back(Mx);
			for(auto& x : ket_evolver->getParams()){ temp.push_back(x.second); }
			temp.push_back(args->getString("Evolver"));
			temp.push_back(args->getString("Geometry"));
			temp.push_back(args->getBool("CenterSite"));
			temp.push_back(args->getBool("Connected"));
			temp.push_back(args->getBool("tune"));
			temp.push_back(args->getBool("disentangle"));
			temp.push_back(args->getBool("ConserveQNs"));
			temp.push_back(args->getBool("ConserveSz"));
			results.push_back(temp);
		}
		return results;
	}

};
#endif