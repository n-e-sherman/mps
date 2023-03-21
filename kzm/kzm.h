#ifndef __KZM_H_
#define __KZM_H_

#include "itensor/all.h"
#include "service/service.h"
#include "addons/itensor-itdvp-util.h"
#include "addons/itensor-itdvp.h"

/* Are all of these headers needed? */
// #include <chrono>
// #include <random>
// #include <omp.h>
// #include <stdlib.h>
// #include <sstream>
// #include <iostream>
// #include <iomanip>
/* -------------------------------- */

using namespace itensor;
using namespace std;

class KZM : public Service
{
protected:

	// Change all the names eventually to be more inline with your coding practice.
	Real beta_tau;
	Real tau;
	Real J;
	Real g0;
	Real gf;
	Real v;
	Real t_f;
	Real time;
	Real n_ex;

	int chi;
	int beta_sweeps;

	iMPS iA;
	ITensor W;
	Index wl;
	Index wr;
	SpinHalf sites;

	vector<Real> Es;
	vector<Real> Eps;
	vector<Real> gs;
	vector<Real> ns;

public:

	KZM(Args* a) : Service(a) 
	{ 
		set_params(); 
	}

	KZM(Args* a, SpinHalf _sites) : Service(a), sites(_sites)
	{
		/* 
		 * Currently an issue in this, need itensor-new 
		 * for KZM so that we can have numcenter =/= 1. 
		 * May be other issues as well, also 
		 * randomize(ITensor,args) is deprecated in favor of .randomize(args) 
		 */

		/*-------initialize the system-------*/
		set_params();
		ITensor A; 
		Index il, ir, is;
		init_MPS(sites, A, il, ir, is, chi); // Could be generalized
		init_MPO(sites, W, wl, wr, g0); // Could be generalized   
		iA = iMPS(A, il, ir, is, W, wl, wr, "A"); 
		auto gs_tol = args->getReal("gs_tol",1E-7);
		
		// Get Ground state
		auto counter = 0;
		for(int i = 0; i < beta_sweeps; i++)
		{
			counter = i;
			iA.iTDVP(-beta_tau, {"MaxIter",30}); // I think this assumes TFIM?
			if( abs(exact_energy(g0) - iA.get_energy(false)) < gs_tol) break;
		}
		cout << "***********************************************************************" << endl;
		cout << "***********************************************************************" << endl;
		cout << "Ground state found in " << counter << " iterations." << endl;
		cout << "***********************************************************************" << endl;
		cout << "***********************************************************************" << endl;
		time = 0;
		auto Ep = iA.get_energy(false);
		auto n = (Ep-exact_energy(g0))/(2.0*J);
		Eps.push_back(Ep);
		Es.push_back(exact_energy(g0));
		gs.push_back(g0);
		ns.push_back(n);

		cout << "(g,t,E,E_exact) = (" << g0 << " ," << time << " ," << iA.get_energy(false) << " ," << exact_energy(g0) << ")" << endl;
	}

	virtual void calculate()
	{

		time = min(time + tau,t_f);
		auto g = gt(time);
		// if(!(g == 2))
		// 	return;
		init_MPO(sites, W, wl, wr, g); // Could be generalized
		iA.set_MPO(W, wl, wr);

		cout << "***********************************************************************" << endl;
		cout << "***********************************************************************" << endl;
		cout << "g = " << g << endl;
		cout << "***********************************************************************" << endl;
		cout << "***********************************************************************" << endl;
		// iA.test();

		iA.iTDVP(-tau*Complex_i, *args);
		// auto Ep = iA.get_energy(false);
		// auto n = (Ep-exact_energy(g))/(2.0*J);
		// Eps.push_back(Ep);
		// Es.push_back(exact_energy(g));
		// gs.push_back(g);
		// ns.push_back(n);
		// cout << "(g, n, n_ex) = (" << g << " ," << n << " ," << n_ex << " )" << endl;
	}
	
	Real getTime() { return time; }
	

	static string getHash(Args* args)
	{
		vector<string> reals{"beta-tau","beta-sweeps","time-tau","g0","MaxDim","v"};
		auto res =  "KZM" + hash_real(reals,args);
		return res;
	}

	virtual void load(ifstream & f)
    {
    	read(f,iA); // May not need this
    	read(f,Es);
    	read(f,Eps);
    	read(f,gs);
    	read(f,ns);
    	read(f,time);
    	read(f,sites);
    }

	virtual void save(ofstream & f)
	{ 
		write(f,iA); // May not need this
    	write(f,Es);
    	write(f,Eps);
    	write(f,gs);
    	write(f,ns);
    	write(f,time);
    	write(f,sites);
	}

private:

	void set_params()
	{
		chi = args->getInt("MaxDim");
		beta_tau = args->getReal("beta-tau");
		beta_sweeps = args->getInt("beta-sweeps");
		tau  = args->getReal("time-tau");
		J = args->getReal("J");
		g0  = args->getReal("g0");
		gf = args->getReal("gf");
		v = args->getReal("v");
		t_f = (g0-gf)/v;
		n_ex = (sqrt(2)/(4.0*M_PI))*sqrt(v);
	}

	double gt(double t)
	{
		return g0 - v*t;
	}

	double exact_energy(double g)
	{
		// return -1.0*(2.0/M_PI)*(1.0+g)*comp_ellint_2(sqrt((4.0*g)/((1.0 + g)*(1.0 + g))));
		/* Commenting out for now. */
		return 0;
	}

	void init_MPS(SiteSet& sites, ITensor& A, Index& il, Index& ir, Index& is, int m)   
	{
		//initialize an open chain with 3 sites. 
		//take the middle one as our system
		auto state = InitState(sites);
		state.set(1,"Up");
		state.set(2,"Up");
		state.set(3,"Up"); 

		auto psi = MPS(state);
		psi = expand_dim(psi, m); 
		A = psi(2); 
		il = commonIndex(A, psi(1));  
		ir = commonIndex(A, psi(3));  
		is = findIndex(A, "Site"); 
	}

	void init_MPO(SiteSet& sites, ITensor& W, Index& wl, Index& wr, double gx) // inputs are sites, and gx, outputs are the other ones
	{
		AutoMPO ampo = AutoMPO(sites); 
		for(int j = 1; j < 3; ++j)
		{
			ampo += -4*J,"Sz",j,"Sz",j+1;
			ampo += -2*gx,"Sx",j;
		}
		ampo += -2*gx,"Sx",3;

		auto H = toMPO(ampo); 
		W = H.ref(2); 
		schur(W); 
		wl = commonIndex(W, H(1)); 
		wr = commonIndex(W, H(3));
	}

	MPS expand_dim(MPS psi, int m) //expand the bond dimension of psi to a new MPS while keep all the tags
	{
		int l = length(psi); 
		vector<Index> links_old(l+1); 
		links_old[0] = findIndex(uniqueInds(psi.ref(1).inds(), psi.ref(2).inds()), "Link");  
		links_old[l] = findIndex(uniqueInds(psi.ref(l).inds(), psi.ref(l-1).inds()), "Link");  

		for(int j = 1; j < l; j++)
		{
			links_old[j] = commonIndex(psi.ref(j), psi.ref(j+1)); 
		}

		vector<Index> links_new(l+1); 
		for(int j = 0; j <= l; j++)
		{
			auto i = Index(m); 
			i.setTags(links_old[j].tags()); 
			links_new[j] = i; 
		}

		for(int j = 1; j <= l; j++)
		{
			auto is = findIndex(psi.ref(j).inds(), "Site");
			ITensor A; 
			A = ITensor(links_new[j-1], is, links_new[j]); 
			A.set(1, 1, 1, 1.0);      
			psi.ref(j) = A; 
		}
		return psi; 
	}

	void schur(ITensor& W) 
	{
		//AutoMPO doesn't produce MPO in Schur form
		//we have to manualy put W in Schur form....
		//the code is written for TFIM
		//The Index id does not change
		int i1 = 2; 
		int i2 = 3; 
		Index a = W.inds().index(1);  
		Index b = W.inds().index(2); 

		ITensor swap_a = ITensor(a, prime(a)); 
		swap_a.set(a(i1), prime(a)(i2), 1); 
		swap_a.set(a(i2), prime(a)(i1), 1); 
		for(int i = 1; i <= dim(a); i++)
		{
			if(i != i1 && i != i2)
			{
				swap_a.set(a(i), prime(a)(i), 1); 
			}
		}

		ITensor swap_b = ITensor(b, prime(b)); 
		swap_b.set(b(i1), prime(b)(i2), 1); 
		swap_b.set(b(i2), prime(b)(i1), 1); 
		for(int i = 1; i <= dim(b); i++)
		{
			if(i != i1 && i != i2)
			{
				swap_b.set(b(i), prime(b)(i), 1); 
			}
		}

		W = (W * swap_a) * swap_b; 
		W.noPrime(prime(a)); 
		W.noPrime(prime(b)); 
	}

	virtual vector<string> _labels()
	{
		auto labels = vector<string>{"v,n,Ep,E,MaxDim,g,beta-tau,beta-sweeps,time-tau"};
		return labels;
	}
	virtual vector<vector<StringReal>> _results()
	{
		auto results = vector<vector<StringReal>>();
		for(int i : range(gs.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(args->getReal("v"));
			temp.push_back(ns[i]);
			temp.push_back(Eps[i]);
			temp.push_back(Es[i]);
			temp.push_back(args->getInt("MaxDim"));
			temp.push_back(gs[i]);
			temp.push_back(args->getReal("beta-tau"));
			temp.push_back(args->getInt("beta-sweeps"));
			temp.push_back(args->getReal("time-tau"));
			results.push_back(temp);
		} 
		return results;
	}
};

#endif
