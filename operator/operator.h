#ifndef __OPERATOR_H_
#define __OPERATOR_H_


#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
#include "infrastructure/util.h"

using namespace itensor;
using namespace std;

class Operator
{
protected:
	/* Inputs */
	Args* args;

	/* Outputs */
	MPO O;


public:
	Operator(){}
	Operator(Args* a) 
	{
		args = a;
	}
	MPO& getO(){ return O;}
	virtual MPO& build(Real qfactor) = 0;

	// static string getHash(Args* args)
	// {	
	// 	return "trash";
	// }
	
};
#endif