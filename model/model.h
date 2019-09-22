#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "lattice/lattice.h"
#include "infrastructure/util.h"
#include "model/modelbuilder.h"
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
	Model(Args* a){args = a; }
	Model(Args* a, Lattice* l, SiteSet s) 
	{ 
		args = a;
		lattice = l; 
		sites = s;
	}
	virtual ~Model(){}
	MPO& getH() {return H; }
	SiteSet getSites() const {return sites;}
	static string getHash(Args* args, int mType = 0)
	{
		return Lattice::getHash(args) + "_" + args->getString("Model") + "_" + args->getString("SiteSet") + "_" + to_string(mType);
	}

	virtual void load(ifstream & f)
	{
		read(f,H);
		auto sType = args->getString("SiteSet");
		if     (sType == "SpinHalf"){ sites = SpinHalf(); sites.read(f); }
		else if(sType == "SpinOne") { sites = SpinOne(); sites.read(f);  }
		else if(sType == "SpinTwo") { sites = SpinTwo(); sites.read(f);  } 

	}
	virtual void save(ofstream & f)
	{
		write(f,H);
		write(f,sites);
	}


};

#endif
