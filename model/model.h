#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
// #include "heisenbergchain.h"
#include "infrastructure/util.h"
#include <cmath>
#include <string>
 #include <typeinfo>

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
	MPO const * const getH() const {return &H; }
	SiteSet getSites() const {return sites;}
	static string getHash(Lattice* l, SiteSet s)
	{
		return l->getHash() +  "_" + to_string(s.si(1).dim()); // This is not unique if you do fermions, may have to change later.
	}
	virtual string getHash() {return Model::getHash(lattice,sites); }


};

#endif
