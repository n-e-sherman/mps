#ifndef __OPERATOR_H_
#define __OPERATOR_H_

#include "state/state.h"
#include "sites/sites.h"
#include <string>
#include <iostream>

using namespace itensor;
using namespace std;

class Operator
{
protected:

	Args* args;
	Sites* sites;
	MPO O;

public:

	Operator(Args* a, Sites* s) : args(a), sites(s) {}
	MPO& getO(){ return O; }

	virtual State multiply(State& s_in, Args* args_in = nullptr)
	{
		if (args_in != nullptr)
			return State(noPrime(applyMPO(O,s_in.getState(),*args_in)));
		return State(noPrime(applyMPO(O,s_in.getState(),*args)));
	}

	void read(istream& is)
    {
    	sites->read(is);
    }
    
    void write(ostream& os) const
    {
    	sites->write(os);
    }
	
};
#endif