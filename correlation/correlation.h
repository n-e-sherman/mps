#ifndef __CORRELATION_H_
#define __CORRELATION_H_

#include "correlation/correlationbuilder.h"

class Correlation
{
protected:
	/* Inputs */
	Args* args;
	State* state;
	Evolver* evolver;
	Model* model;

	MPS psi;
	MPS psiB;

	Real tau;
	Real time;
	vector<Real> times;
	vector<string> labels;
	vector<vector<StringReal>> results;
	

public:
	Correlation(Args* a, Evolver* e, Model* m) : args(a), evolver(e), model(m) {}
	Correlation(Args* a, State* s, Evolver* e, Model* m) : args(a), state(s), evolver(e), model(m)
	{
		tau = args->getReal("tau");
		time = 0;
		times.push_back(time);
		psi = state->getState();
		psiB = MPS(psi);
		evolver->setup(BondGate::tReal, tau);
	}

	Real getTime() { return time; }
	virtual void calculate() = 0;
	virtual void processResults() = 0;
	virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
	{
		processResults();
		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
	}

	static string getHash(Args* args)
	{ 
		return State::getHash(args) + "_Correlation" + "_" + to_string(args->getBool("momentum")) + "_" + to_string(args->getReal("tau")) + "_" + args->getString("Evolver");
	}

	virtual void load(ifstream & f)
    {
		read(f,psi);
		read(f,psiB);
		read(f,time);
		read(f,tau);
		read(f,times);
		psi.position(1);
		psiB.position(1);
		evolver->setup(BondGate::tReal, tau);
    }

	virtual void save(ofstream & f)
	{
		write(f,psi);
		write(f,psiB);
		write(f,time);
		write(f,tau);
		write(f,times);
	}
};
#endif