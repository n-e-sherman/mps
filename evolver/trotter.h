#ifndef __TROTTER_H_
#define __TROTTER_H_

#include "evolver/evolver.h"

using namespace itensor;
using namespace std;

class Trotter : public Evolver
{
protected:

	vector<Model::gate> mgates;
	vector<BondGate> gates;
	Sites* sites;

public:

	Trotter(){}
	Trotter(Args* a, Model* m) : Evolver(a,m)
	{
		sites = model->getSites();
	}

	void evolve(State& s)
	{
		gateTEvol(gates,1.0,1.0,s.getState(),*args); // time evolves, don't need -t, just t, this figures it out.
	}

	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		if(op == "H"){ mgates = model->getGatesH();	}
		else         { mgates = model->getGatesL(); }
		// if(type != BondGate::tReal)
		// 	args->add("Normalize",true);
		buildGates(type,tau);
	}

	virtual void read(istream& is)
	{
		Evolver::read(is);
		sites = model->getSites();
	}

	virtual void write(ostream& os) const
	{
		Evolver::write(os);
	}

private:

	void buildGates(BondGate::Type type, Real tau) // use actual time, NOT exp argument (-time)
	{
		gates.clear();
		auto theta = 1.0/(2.0 - pow(2.0,1.0/3.0));
		auto thermal = args->getBool("thermal");
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*theta/2.0,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1); // Swap gate, only needed in thermal!!
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);

			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*theta,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
				
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*(1.0-theta)/2.0,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*(1.0-2*theta),x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*(1.0-theta)/2.0,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*theta,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
			}
		}
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				auto g = BondGate(sites->getSites(),x.s1,x.s2,type,tau*theta/2.0,x.t);
				if(thermal)
				{
					auto s1 = BondGate(sites->getSites(),x.s2,x.s2+1);
					gates.push_back(s1);
					gates.push_back(g);
					gates.push_back(s1);
				}
				else
					gates.push_back(g);
			}
		}
	}

};
#endif
