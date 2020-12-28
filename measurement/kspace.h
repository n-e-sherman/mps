#ifndef __KSPACE_H_
#define __KSPACE_H_

#include "measurement/measurement.h"
#include "operator/operator.h"

class KSpace : public Measurement
{
protected:

	Operator* O;

public:

	KSpace(Args* a, Operator* o)  : Measurement(a), O(o) {}

	virtual vector<StringReal> measure(State& bra, State& ket)
	{
		vector<StringReal> res;
		res.push_back(innerC(bra.getState(),O->getO(),ket.getState()));
		return res;
	}

	virtual vector<string> addLabels()
	{
		auto res = vector<string>();
		res.push_back("S");
		if( args->getBool("imaginary") ) res.push_back("IS");
		res.push_back("qFactor");
		return res;
	}

	virtual vector<StringReal> addResults(vector<StringReal> &_in) 
	{
		auto res = vector<StringReal>();
		for(auto j : range(_in.size())) res.push_back(_in[j].real());
		if(args->getBool("imaginary")) for(auto j : range(_in.size())) res.push_back(_in[j].imag());
		return res;
	}

};
#endif
