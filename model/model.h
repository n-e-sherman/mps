#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
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
	static string getHash(Args* args, ModelBuilder::modelType mType = normal)
	{
		return Lattice::getHash(args) + "_" + args->getString("Model") + "_" + args->getString("SiteSet") + "_" + to_string(mType);
	}

	virtual void load(ifstream & f)
	{
		read(f,H);
		read(f,sites);
	}
	virtual void save(ofstream & f)
	{
		write(f,H);
		write(f,sites);
	}


};

#endif
