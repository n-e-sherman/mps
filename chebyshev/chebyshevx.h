#ifndef __CHEBYSHEVX_H_
#define __CHEBYSHEVX_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "chebyshev/chebyshev.h"
#include "itensor/util/print_macro.h"
#include "measurement/connectedx.h"

#include <chrono>


using namespace std;
using namespace itensor;

class Chebyshevx : public Chebyshev
{
protected:
	Lattice* lattice;
	Measurement* measurement;
	Repository* repo;
	vector<vector<StringReal>> res; // [iteration][site], site starts 0 NOT 1
	vector<vector<StringReal>> resP;
	vector<vector<StringReal>> resA;
	vector<StringReal> connected;
	vector<int> P;
	vector<int> A;
	vector<Real> chis;
	int N;
	

public:

	Chebyshevx(Args* a) : Chebyshev(a){}
	Chebyshevx(Args* a, Model* m, Lattice* l, Sweeper* swp, Measurement* ms, Repository* r) : Chebyshev(a,m,swp), lattice(l), measurement(ms), repo(r)  {}
	Chebyshevx(Args* a, Model* m, State *s, Lattice* l, Sweeper* swp, Measurement* ms, Repository* r) : Chebyshev(a,m,s,swp), lattice(l), measurement(ms), repo(r)
	{
		if(args->getBool("errorMPOProd")) errorMPO = vector<Real>({0,0});
		auto sites = model->getSites();	
		psi = state->getState();
		processSites();
		psi.position(1);
		H.position(1);
		t0 = MPS(psi);
		connected = measurement->calculate(psi,psi); proccessConnected();
		if (args->getBool("measureAll")) { measureAll(psi); }
		N = args->getInt("N");
		int c = N/2;
		auto lOp_E = connected[c].real(); // may want to generalize
		auto lOp = op(sites,args->getString("localOperator"),P[c]);

		auto _s = sites(P[c]);
	    auto sP = prime(_s);
	    auto Up = _s(1); auto UpP = sP(1); auto Dn = _s(2); auto DnP = sP(2);
	    auto lOp_c = ITensor(dag(_s),sP); lOp_c.set(Up,UpP,lOp_E); lOp_c.set(Dn,DnP,lOp_E);
	    auto lOp_T = lOp-lOp_c;

		t0.position(P[c]);
		auto temp = t0(P[c]) * lOp_T;
		temp.noPrime();
		t0.set(P[c],temp);
		t0.position(1);
		res.push_back(measurement->calculate(psi,t0));
		if (args->getBool("measureAll")) { measureAll(t0); }
		t1 = noPrime(applyMPO(H,t0));
		t2 = t1;
		res.push_back(measurement->calculate(psi,t1));
		if(args->getBool("measureAll"))	{ measureAll(t1); }
		is = siteInds(t0);
		iteration = 1;
	}
	
	~Chebyshevx(){}
	virtual void calculate(int iterations)
	{
		for(auto i : range1(iterations))
		{
			cout << iteration + i << endl;
			auto temp = noPrime(applyMPO(H,t1));
			if(args->getBool("errorMPOProd")) errorMPO.push_back(errorMPOProd(temp,H,t1));
			temp *= 2;
			prepare(temp,t0,is);
			t2 = sum(temp,-1*t0,*args);
			sweeper->sweep(H,t2);
			if(args->getBool("details")) details.push_back(sweeper->get_details());
			res.push_back(measurement->calculate(psi,t2));
			if(args->getBool("measureAll"))	{ measureAll(t2); }
			t0 = t1;
			t1 = t2;
		}
		iteration += iterations;
	}

	void measureAll(MPS & psi_in)
	{
		auto [_resP,_resA] = measurement->calculateAll(psi_in);
		resP.push_back(_resP);
		resA.push_back(_resA);
		chis.push_back(maxLinkDim(psi_in));
		proccesResultsAP(resP,1);
		repo->save(Chebyshev::getHash(args),args->getString("localOperator")+"x"+"/"+args->getString("Model"),labels,results);
		proccesResultsAP(resA,0);
		repo->save("Ancilla"+Chebyshev::getHash(args),args->getString("localOperator")+"x"+"/"+args->getString("Model"),labels,results);
	}

	void processSites()
	{	
		P.push_back(0);
		A.push_back(0);
		for(auto x : lattice->getSites())
		{
			if(x.t == Lattice::physical) 
			{
				P.push_back(x.s);
			}
			else{ A.push_back(x.s); }
		}
	}

	void processResults()
	{
		/* res in formalt [moment], want to add settings to labels and results */
		labels.clear();
		results.clear();
		for(auto i : range1(args->getInt("N"))) labels.push_back(to_string(i));
		if(args->getBool("imaginary")) for(auto i : range1(args->getInt("N"))) labels.push_back("I" + to_string(i));
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		labels.push_back("W");
		labels.push_back("Wp");
		for(auto& x : model->getParams()){ labels.push_back(x.first); }
		if(args->getBool("errorMPOProd")){ labels.push_back("errorMPOProd"); }
		if ((args->getString("sweeperType") == "exact") || (args->getString("sweeperType") == "projection"))
		{
			labels.push_back("sweeperType");
			labels.push_back("sweeperCount");
			labels.push_back("MaxIter");
			for(auto& x : detail_labels){ labels.push_back(x); }
		}
		
		for(int i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			for(auto j : range(res[i].size())) temp.push_back(res[i][j].real());
			if(args->getBool("imaginary")) for(auto j : range(res[i].size())) temp.push_back(res[i][j].imag());
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			if(args->getBool("errorMPOProd")) temp.push_back(errorMPO[i]);
			if ((args->getString("sweeperType") == "exact") || (args->getString("sweeperType") == "projection"))
			{	
				auto sProj = args->getString("sweeperType");
				temp.push_back(sProj);
				if(sProj == "identity"){ temp.push_back(0.0); temp.push_back(0.0); }
				else
				if(sProj == "exact"){ temp.push_back(args->getReal("sweeperCount")); temp.push_back(0.0); }
				else
				if(sProj == "projection"){ temp.push_back(args->getReal("sweeperCount")); temp.push_back(args->getReal("MaxIter")); }			
				for(auto& x : details[i]){temp.push_back(x); }
			}
			results.push_back(temp);
		}
	}

	void proccessConnected()
	{
		labels.clear();
		results.clear();

		for(auto i : range1(args->getInt("N"))) labels.push_back(to_string(i));
		if(args->getBool("imaginary")) {for(auto i : range1(args->getInt("N"))) labels.push_back("I" + to_string(i));}
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		if(args->getBool("thermal")) { labels.push_back("beta"); labels.push_back("tau"); }
		for(auto& x : model->getParams()){ labels.push_back(x.first); }

		auto temp = vector<StringReal>();
		for(auto j : range(connected.size())) temp.push_back(connected[j].real());
		if(args->getBool("imaginary")) for(auto j : range(connected.size())) temp.push_back(connected[j].imag());
		temp.push_back(args->getReal("MaxDim"));
		temp.push_back(args->getReal("N"));
		temp.push_back(args->getString("Lattice"));
		temp.push_back(args->getString("Model"));
		temp.push_back(args->getBool("thermal"));
		if(args->getBool("thermal")) { temp.push_back(args->getReal("beta")); temp.push_back(args->getReal("tau")); }
		for(auto& x : model->getParams()){ temp.push_back(x.second); }
		results.push_back(temp);
		repo->save(Chebyshev::getHash(args),"chebyshevx/"+args->getString("Model")+"/connected",labels,results);
	}

	void proccesResultsAP(vector<vector<StringReal>> resAP, int physical)
	{
		labels.clear();
		results.clear();
		for(auto i : range1(args->getInt("N"))) labels.push_back(to_string(i));
		if(args->getBool("imaginary")) for(auto i : range1(args->getInt("N"))) labels.push_back("I" + to_string(i));
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		labels.push_back("physical");
		labels.push_back("chi");
		for(auto& x : model->getParams()){ labels.push_back(x.first); }

		for(auto i : range(resAP.size()))
		{
			auto temp = vector<StringReal>();
			for(auto j : range(resAP[i].size())) temp.push_back(resAP[i][j].real());
			if(args->getBool("imaginary")) for(auto j : range(resAP[i].size())) temp.push_back(resAP[i][j].imag());
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			temp.push_back(physical);
			for(auto& x : model->getParams()){ temp.push_back(x.second); }
			temp.push_back(chis[i]);
			results.push_back(temp);
		}
	}

	virtual void load(ifstream & f)
    {
        Chebyshev::load(f);
        read(f,res);
        read(f,resP);
        read(f,resA);
        read(f,N);
        read(f,P);
        read(f,A);
        read(f,errorMPO);
        read(f,connected);
    }

	virtual void save(ofstream & f)
	{
		Chebyshev::save(f);
		write(f,res);
		write(f,resP);
		write(f,resA);
		write(f,N);
		write(f,P);
		write(f,A);
		write(f,errorMPO);
		write(f,connected);
	}

};

#endif