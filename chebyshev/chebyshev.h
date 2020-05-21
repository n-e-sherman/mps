#ifndef __CHEBYSHEV_H_
#define __CHEBYSHEV_H_
#include <vector>
#include <string>
#include "itensor/all.h"


using namespace std;
using namespace itensor;

class Chebyshev
{
protected:
	Args* args;
	Model* model;
	State* state;
	MPO H;
	bool thermal;
	int nChebyshev;
	Real W;
	Real Wp;

	/* Helpers */
	MPS psi;
	MPS t0;
	MPS t1;
	MPS t2;
	IndexSet is;
	int iteration;

	/* Outputs */
	vector<string> labels;
	vector<vector<StringReal>> results;	

public:

	Chebyshev(Args* a) {args = a;}
	Chebyshev(Args* a, Model* m){args = a; model = m;}
	Chebyshev(Args* a, Model* m, State *s)
	{ 
		args = a;
		model = m;
		state = s;
		args = a;
		thermal = args->getBool("thermal");
		W = args->getReal("W");
		Wp = args->getReal("Wp");
		if(thermal) 
		{
			H = model->getL();
			H *= (2.0*Wp/W);
		}
		else 
		{
			H = model->getH();
			// H *= (2.0*Wp/W);  // Only works for thermal, need different scaling for ground 
			throw runtime_error("Have not implemented chebyshev for ground state calculation.");
		}
	}
	
	~Chebyshev(){}
	virtual void calculate(int iterations) = 0;
	virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
	{
		processResults();
		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
	}
	virtual int getIteration(){ return iteration; }
	virtual void processResults() = 0;




	static string getHash(Args* args)
	{ /* This needs some love to be a hash function inline with what you had before, maybe different for saving results? that can prob be in service. */
		return State::getHash(args) + "_" + to_string(args->getReal("W")) + "_Chebyshev" + "_" + to_string(args->getBool("momentum")) + "_" + to_string(args->getReal("W"));
	}

	virtual void load(ifstream & f)
	{
		read(f,psi);
		read(f,t0);
		read(f,t1);
		read(f,t2);
		read(f,H);
		read(f,W);
		read(f,Wp);
		read(f,thermal);
		read(f,iteration);
		psi.position(1);
		t0.position(1);
		t1.position(1);
		t2.position(1);
		H.position(1);
		is = siteInds(t0);

	}
	virtual void save(ofstream & f)
	{
		write(f,psi);
		write(f,t0);
		write(f,t1);
		write(f,t2);
		write(f,H);
		write(f,W);
		write(f,Wp);
		write(f,thermal);
		write(f,iteration);
	}





};

#endif