#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "model/sitebuilder.h"
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
		if     (sType == "SpinHalf"){ auto rSites = SpinHalf(args->getInt("N")); rSites.read(f); sites = rSites; }
		else if(sType == "SpinOne") { auto rSites = SpinOne(args->getInt("N")); rSites.read(f); sites = rSites;  }
		else if(sType == "SpinTwo") { auto rSites = SpinTwo(args->getInt("N")); rSites.read(f); sites = rSites;  } 

	}
	virtual void save(ofstream & f)
	{
		write(f,H);
		write(f,sites);
	}


};

#endif
