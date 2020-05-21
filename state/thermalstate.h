#ifndef __THERMALSTATE_H_
#define __THERMALSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include <cmath>

using namespace itensor;
using namespace std;

class ThermalState : public State
{
protected:
	Real beta;

	void calcThermalState()
	{
		calcPsii();
		if(beta > 0) coolPsii();
		else{ }
	}
	
	void calcPsii() // Makes an infinite temperature state via purification.
	{
		auto sites = model->getSites();
		state = MPS(sites);
		// state = removeQNs(state);
		auto N = sites.length();
    	for(int n = 1; n <= N; n += 2)
        {
	        auto s1 = sites(n);
	        auto s2 = sites(n+1);
	        auto wf = ITensor(s1,s2);
	        wf.set(s1(1),s2(2), ISqrt2);
	        wf.set(s1(2),s2(1), -ISqrt2);
	        ITensor D;
	        state.Aref(n) = ITensor(s1);
	        state.Aref(n+1) = ITensor(s2);
	        svd(wf,state.Aref(n),D,state.Aref(n+1));
	        state.Aref(n) *= D;
        }
        // state.orthogonalize();
        // state.normalize();
	}

	void coolPsii()
	{
		auto tau = args->getReal("tau");
		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	auto maxDim = args->getInt("MaxDim");
    	args->add("MaxDim",args->getInt("thermalMaxDim"));
    	if(fabs(nt*tau-ttotal) > eps)
    	{
    		nt += 1;
    		tau = ttotal/Real(nt);
    	}
    	args->add("tau",tau);

		auto type = args->getString("coolingType");
		if(type == "Trotter") Trotter();
		if(type == "MPO") MPOCool();
		cout << "cooling done." << endl;
        // state.orthogonalize();
        state.normalize();
        args->add("MaxDim",maxDim);
	}

	void Trotter()
	{
		// NOTE: You are assuming that the state is just a chain with an ancilla
		cout << "Starting Trotter. " << endl;
		auto sites = model->getSites();
		auto mgates = model->getGatesH();
		auto gates = vector<BondGate>();
		auto tau = args->getReal("tau");
		auto theta = 1.0/(2.0 - pow(2.0,1.0/3.0));
		auto N = args->getInt("N");
		// int eCut = 0;
		// int oCut = 0;
		// if(N%2 == 0) // even
		// {
		// 	eCut = 2*(N-1)-3;
		// 	oCut = 2*N-3;
		// }
		// else
		// {
		// 	oCut = 2*(N-1)-3;
		// 	eCut = 2*N-3;
		// }
		cout << "even" << endl;
		for(auto x : mgates)
		{
			if(x.l == "even")
			{	
				cout << x.s1 << "," << x.s2 << "," << x.s2+1 << "," << sites.length() << endl;
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*theta/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		cout << "odd" << endl;
		for(auto x : mgates)
		{
			if(x.l == "odd")
			{	
				cout << x.s1 << "," << x.s2 << "," << x.s2+1 << "," << sites.length() << endl;
				auto s1 = BondGate(sites,x.s2,x.s2+1);
				gates.push_back(s1);
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*theta,x.t);
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
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*(1.0-theta)/2.0,x.t);
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
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*(1.0-2*theta),x.t);
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
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*(1.0-theta)/2.0,x.t);
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
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*theta,x.t);
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
				auto g = BondGate(sites,x.s1,x.s2,BondGate::tImag,tau*theta/2.0,x.t);
				gates.push_back(g);
				gates.push_back(s1);
			}
		}
		cout << "Time evolving. " << endl;

		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	for(int tt=1; tt <= nt; ++tt) gateTEvol(gates,tau,tau,state,*args);
	}

	void MPOCool()
	{ 
    	auto realStep = args->getBool("realStep");
		auto tau = args->getReal("tau");
		auto ttotal = beta/2.0;
		auto eps = args->getReal("thermalEps");
    	int nt = int((ttotal/tau)*(1.0+eps));
    	if(realStep)
    	{
	        auto expH = model->getExpH(tau);
	        for(int tt = 1; tt <= nt; ++tt)
	        {
	        	state = noPrime(applyMPO(expH,state,*args));
	        }
    	}
    	else
    	{
	    	auto taua = tau/2.*(1.+1._i);
	        auto taub = tau/2.*(1.-1._i);
	        auto expHa = model->getExpH(taua);
	        auto expHb = model->getExpH(taub);
	        for(int tt = 1; tt <= nt; ++tt)
	        {
	        	state = noPrime(applyMPO(expHa,state));
	        	state = noPrime(applyMPO(expHb,state));
	        }
    	}
	}

public:

	ThermalState(){}
	ThermalState(Args* a, Model* m) : State(a,m)
	{
		beta = args->getReal("beta");
		calcThermalState();
	}
	~ThermalState() {}	
};
#endif