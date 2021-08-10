#ifndef __CENTERSITE_H_
#define __CENTERSITE_H_

#include "measurement/measurement.h"
#include "operator/operator.h"

/* Idea:
 * 	- Use the position operator to define an S_c operator, with the disconnected contribution accounted for.
 * 	- in 'Measure', apply that op to the bra, and then return the inner product between the result, and ket.
 *	- Will have to think a bit about the accounting on this measurement for things like labels and io etc.
 */

class CenterSite : public Measurement
{
protected:

	Operator* O;

public:

	CenterSite(Args* a, Operator* o)  : Measurement(a), O(o) {}

	virtual vector<StringReal> measure(State& bra, State& ket)
	{
		vector<StringReal> res;
		auto Obra = O->multiply(bra);
		res.push_back(innerC(Obra.getState(),ket.getState()));
		return res;
	}

	virtual vector<string> addLabels()
	{
		auto res = vector<string>();
		res.push_back("S");
		if( args->getBool("imaginary") ) res.push_back("IS");
		res.push_back("qx");
		res.push_back("qy");
		res.push_back("qz");
		res.push_back("momentumFunction");
		// res.push_back("qFactor");
		return res;
	}

	virtual vector<StringReal> addResults(vector<StringReal> &_in) 
	{
		auto res = vector<StringReal>();
		for(auto j : range(_in.size())) res.push_back(_in[j].real());
		if(args->getBool("imaginary")) for(auto j : range(_in.size())) res.push_back(_in[j].imag());
		res.push_back(args->getReal("qx"));
		res.push_back(args->getReal("qy"));
		res.push_back(args->getReal("qz"));
		res.push_back(args->getString("momentumFunction"));
		return res;
	}

	virtual void read(istream& is) 
	{
		O->read(is);
	}
    virtual void write(ostream& os) const 
    {
    	O->write(os);
    }

};
#endif
