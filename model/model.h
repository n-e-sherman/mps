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
#include <optional>

using namespace itensor;
using namespace std;

class Model
{
public:
	struct gate
	{
		int s1 = 0;
		int s2 = 0;
		ITensor t;
		string l = "";
	};
protected:

	/* Inputs */
	Args* args;
	Lattice* lattice;

	/* Helpers */
	Cplx tauEH;
	Cplx tauEL;

	/* Outputs */
	SiteSet sites;
	map<string, Real> params;

	optional<AutoMPO> ampoH;
	optional<MPO> H;
	optional<MPO> expH;
	vector<gate> gatesH;
	// GateH

	optional<AutoMPO> ampoL;
	optional<MPO> L;
	optional<MPO> expL;
	vector<gate> gatesL;
	// GateL


	/* Methods */
	virtual void calcH(){ if(!ampoH) calcAmpoH(); H = toMPO(*ampoH); }
	virtual void calcExpH(Cplx tau){ if(!ampoH) calcAmpoH(); expH = toExpH(*ampoH,tau); }
	virtual void calcAmpoH() {};
	virtual void calcGatesH() {};

	virtual void calcL(){ if(!ampoL) calcAmpoL(); L = toMPO(*ampoL); }
	virtual void calcExpL(Cplx tau){ if(!ampoL) calcAmpoL(); expL = toExpH(*ampoL,tau);	}
	virtual void calcAmpoL() {};
	virtual void calcGatesL() {};



public:

	Model(){}
	Model(Args* a, Lattice* l) : args(a), lattice(l) {}
	Model(Args* a, Lattice* l, SiteSet s) : args(a), lattice(l), sites(s) {}

	virtual ~Model(){}
	
	MPO& getH() { if(!H) calcH(); return *H; }
	MPO& getExpH(Cplx tau, bool force = false) { if(!expH || force) calcExpH(tau); tauEH = tau; return *expH; } // force ensures a new calculation.
	AutoMPO& getAmpoH() {if(!ampoH) calcAmpoH(); return *ampoH; } // not needed?
	vector<gate> getGatesH() {if(gatesH.size() == 0) calcGatesH(); return gatesH; }

	MPO& getL() { if(!L) calcL(); return *L; }
	MPO& getExpL(Cplx tau, bool force = false) { if(!expL || force) calcExpL(tau); tauEL = tau; return *expL; } // force ensures a new calculation.
	AutoMPO& getAmpoL() {if(!ampoL) calcAmpoL(); return *ampoL; }
	vector<gate> getGatesL() {if(gatesL.size() == 0) calcGatesL(); return gatesL; }



	map<string, Real> getParams() {return params; }
	SiteSet getSites() const {return sites; }

	static string hashParams(Args* args)
	{
		/* May want to wait on this change, as it will not load old hashing, but this new method is better */
		/***************************************************************************************************/
		string res = "";
		auto sModel = args->getString("Model");
		if(sModel == "XXZ")
			res = to_string(args->getReal("Jxy")) + to_string(args->getReal("Delta"));
		if(sModel == "Dimer")
			res = to_string(args->getReal("Je")) + to_string(args->getReal("Jo"));
		if(sModel == "HeisenbergField")
			res = to_string(args->getReal("B"));
		/***************************************************************************************************/
		/* May want to wait on this change, as it will not load old hashing, but this new method is better */
		return res;
	}

	static string getHash(Args* args)
	{
		auto sParams = Model::hashParams(args);
		return Lattice::getHash(args) + "_" + args->getString("Model") + "_" + args->getString("SiteSet") + "_" + sParams;
	}

	virtual void load(ifstream & f)
	{
		auto sType = args->getString("SiteSet");
		if     (sType == "SpinHalf"){ auto rSites = SpinHalf(args->getInt("N")); rSites.read(f); sites = rSites; }
		else if(sType == "SpinOne") { auto rSites = SpinOne(args->getInt("N")); rSites.read(f); sites = rSites;  }
		else if(sType == "SpinTwo") { auto rSites = SpinTwo(args->getInt("N")); rSites.read(f); sites = rSites;  } 
	}
	virtual void save(ofstream & f)
	{
		write(f,sites);
	}

	
};

#endif
