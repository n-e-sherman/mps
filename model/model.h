#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
// #include "heisenbergchain.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Model
{
protected:
	InputGroup* input;
	MPO H;
	Lattice* lattice;
	SiteSet sites;

public:
	Model(){}
	Model(Lattice* l, SiteSet s, InputGroup* i) 
	{ 
		input = i;
		lattice = l; 
		sites = s;
	}
	virtual ~Model(){}
	MPO const * const getH() const {return &H;}
	SiteSet getSites() const {return sites;}

};

#endif
