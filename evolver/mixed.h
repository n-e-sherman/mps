#ifndef __MIXED_H_
#define __MIXED_H_

#include "evolver/evolver.h"
#include "addons/itensor-tdvp.h"

using namespace itensor;
using namespace std;

class Mixed : public Evolver
{
protected:

	/* Trotter */
	vector<Model::gate> mgates;
	vector<BondGate> gates;
	Sites* sites;

	/* tdvp */
	Sweeps sweeps;
	Cplx dt;

public:

	Mixed(){}
	Mixed(Args* a, Model* m) : Evolver(a,m)
	{
		sites = model->getSites();
	}

	void evolve(State& s)
	{
		if(s.maxLinkDim() < args->getInt("MaxDim")) // use TEBD until chi saturated, then use TDVP.
		{
			cout << s.maxLinkDim() << endl;
			gateTEvol(gates,1.0,1.0,s.getState(),*args);
		}
		else
			tdvp(s.getState(),model->getO(),-dt,sweeps,*args);
	}

	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		dt = Cplx(tau);
		if(type == BondGate::tReal)
		{
			dt *= Complex_i;
		}
		if(op == "H"){ model->calcH(); mgates = model->getGatesH(); }
		else         { model->calcL(); mgates = model->getGatesL(); }

		buildGates(type,tau);

		/* Make these args parameters? */
		sweeps = Sweeps(1);
	    sweeps.maxdim() = args->getInt("MaxDim");
	    sweeps.niter() = args->getInt("niter");
	}

	virtual void read(istream& is)
	{
		sites->read(is);
	}

	virtual void write(ostream& os) const
	{
		sites->write(os);
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