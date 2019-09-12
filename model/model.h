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
	/* Inputs */
	Args* args;
	Lattice* lattice;

	/* Outputs */
	MPO H;
	SiteSet sites;

public:
	Model(){}
	Model(Args* a, Lattice* l, SiteSet s) 
	{ 
		args = a;
		lattice = l; 
		sites = s;
	}
	virtual ~Model(){}
	MPO& getH() const {return &H; }
	SiteSet getSites() const {return sites;}
	static string getHash(Lattice* l, SiteSet s)
	{
		return l->getHash() +  "_" + to_string(s.si(1).dim()); // This is not unique if you do fermions, may have to change later.
	}
	virtual string getHash() {return Model::getHash(lattice,sites); }


};

#endif
