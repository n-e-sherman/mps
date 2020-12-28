#ifndef __MEASUREMENT_H_
#define __MEASUREMENT_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include "state/state.h"

using namespace itensor;
using namespace std;

class Measurement
{
protected:

	Args* args;

public:

	Measurement(Args* a) : args(a) {}
	virtual vector<StringReal> measure(State& ket){ return measure(ket,ket); }
	virtual vector<StringReal> measure(State& bra, State& ket)
	{
		vector<StringReal> res;
		res.push_back(innerC(bra.getState(),ket.getState()));
		return res;
	}
	
	virtual vector<string> addLabels() = 0;
	virtual vector<StringReal> addResults(vector<StringReal> &_in) = 0;
	virtual void update(){}

};
#endif