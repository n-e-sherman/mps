#ifndef __TROTTER_H_
#define __TROTTER_H_

#include "evolver/evolver.h"
#include "model/model.h"

using namespace itensor;
using namespace std;

class Trotter : public Evolver
{
protected:
	/* Inputs */
	Args* args;
	SiteSet sites;
	Model* model;

	/* Helpers */
	vector<Model::gate> mgates;
	vector<BondGate> gates;
	Real theta;
	bool Normalize;
	/* Outputs */


public:
	Trotter(){}
	Trotter(Args* a, Model* m) : args(a), model(m)
	{
		sites = model->getSites();
		theta = 1.0/(2.0 - pow(2.0,1.0/3.0));
	}
	void evolve(MPS & psi)
	{
		gateTEvol(gates,1.0,1.0,psi,*args);
	}
	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		if(op == "H"){ mgates = model->getGatesH();	}
		else         { mgates = model->getGatesL(); }
		buildGates(type,tau);
	}
	void buildGates(BondGate::Type type, Real tau)
	{
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*theta/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*theta,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*(1.0-theta)/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*(1.0-2*theta),x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*(1.0-theta)/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*theta,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,type,tau*theta/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
	}
};
#endif
