#ifndef __MEASUREMENT_H_
#define __MEASUREMENT_H_


#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
#include "state/state.h"
#include "operator/operator.h"
#include "infrastructure/util.h"

using namespace itensor;
using namespace std;

class Measurement
{
protected:

	/* Inputs */
	Args* args;
	State* state;
	Model* model;
	int N;

	/* Outputs */
	vector<string> labels;
	vector<vector<StringReal>> results;

public:

	Measurement(Args* a) : args(a) { N = args->getInt("N"); }
	Measurement(Args* a, Model* m) 
	{ 
		args = a; 
		model = m;
		N = args->getInt("N");
	}
	virtual void processResults() = 0;
	virtual vector<StringReal> calculate(MPS & bra, MPS & psi) = 0;
	virtual vector<StringReal> calculate(MPS & psi) { return calculate(psi,psi); }
	virtual tuple< vector<StringReal>, vector<StringReal> > calculateAll(MPS & bra, MPS & psi) = 0;
	tuple< vector<StringReal>, vector<StringReal> > calculateAll(MPS & psi) { return calculateAll(psi,psi); }
	
	virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
	{
		processResults();
		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
	}

	static string getHash(Args* args)
	{	
		return "trash";
	}
	
};
#endif