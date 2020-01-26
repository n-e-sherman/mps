// #ifndef __MOMENTS_H_
// #define __MOMENTS_H_

// #include "itensor/all.h"
// #include "infrastructure/util.h"
// #include "services/service.h"
// #include "model/modelbuilder.h"
// #include "lattice/latticebuilder.h"
// #include "repository/repositorybuilder.h"
// #include "state/statebuilder.h"
// #include <string>
// #include <iostream>

// using namespace itensor;
// using namespace std;

// class Moments : public Service
// {
// private:
// 	/* Inputs */
// 	Args* args;
// 	ModelBuilder* modelBuilder;
// 	Model* model;
// 	SiteSet sites;
// 	StateBuilder* stateBuilder;
// 	State* state;
// 	LatticeBuilder* latticeBuilder;
// 	Lattice* lattice;
// 	RepositoryBuilder* repoBuilder;
// 	Repository* repo;

// 	/* Helpers */
// 	MPS psi;
// 	MPO H;

// 	/* Outputs */
// 	vector<string> labels;
// 	vector<vector<StringReal>> results;

// public:
// 	Moments() : Service(){}
// 	Moments(ModelBuilder *mb, StateBuilder *sb, LatticeBuilder* lb, RepositoryBuilder* rb) : Service()
// 	{ 
// 		modelBuilder = mb;
// 		repoBuilder = rb;
// 		stateBuilder = sb;
// 		latticeBuilder = lb;
// 	}
// 	~Moments(){}

// 	void calculate(Args* a)
// 	{
// 		args = a;
// 		lattice = latticeBuilder->build(args);
// 		auto thermal = args->getBool("thermal");
// 		if(thermal) 
// 		{
// 			model = modelBuilder->build(args,ModelBuilder::thermal);
// 			state = stateBuilder->build(args,StateBuilder::thermal);
// 		}
// 		else 
// 		{
// 			model = modelBuilder->build(args);
// 			state = stateBuilder->build(args);
// 		}
// 		H = model->getH();
// 		sites = model->getSites();
// 		psi = state->getState();
// 		if(args->getBool("momentum")) { calcMomentum(); labels.push_back("momentum"); }
// 		else {calcPosition(); labels.push_back("position"); }
// 		repo = repoBuilder->build(args);
// 		for(auto n : range(args->getInt("Nmoments") + 1)) labels.push_back(to_string(n));
// 		repo->save(getHash(),"moments",labels,results);

// 	}

// 	void calcMomentum()
// 	{
// 		auto N = args->getInt("N");
//         auto coeff = sqrt(2.0/(Real(N+1)));
//         auto thermal = args->getBool("thermal");
// 		for(auto i : range1(N))
// 		{
// 		    auto ampo = AutoMPO(sites);
// 			auto k = Real(M_PI*i)/Real(N+1);
// 			cout << i << "," << k << endl;
// 	        for(auto s : lattice->getSites())
// 	        {
// 	        	if(s.t == Lattice::physical)
// 	        	{
// 	        		if(thermal){ ampo += coeff*sin(0.5*(s.s+1)*k),"Sz",s.s; }
// 		            else{ ampo += coeff*sin(s.s*k),"Sz",s.s; }
// 		        }
// 	        }
// 	        auto A = toMPO(ampo);
// 	        results.push_back(calcMoments(k, A));
// 		}
// 	}

// 	void calcPosition()
// 	{
// 		auto N = args->getInt("N");
// 		for(auto s : lattice->getSites())
// 		{
// 		    auto ampo = AutoMPO(sites);
// 			if(s.t == Lattice::physical)
// 			{
// 				cout << s.s << endl;
// 				ampo += 1.0,"Sz",s.s;
// 		        auto A = toMPO(ampo);
// 		        results.push_back(calcMoments((s.s+1)/2, A));
// 	    	}
// 		}
// 	}

// 	vector<StringReal> calcMoments(Real in, MPO& A)
// 	{
// 		/* TODO: Implement higher moments. */
// 		auto nMoments = args->getInt("Nmoments");
// 		vector<StringReal> res;
// 		res.push_back(in);
// 		auto PsiA = applyMPO(A,psi);
// 		res.push_back(inner(PsiA,PsiA));

// 		return res;
// 	}




// 	virtual string getHash()
// 	{
// 		string type = "momentum";
// 		if(!args->getBool("momentum")) type = "position";
// 		return State::getHash(args) + "_" + to_string(args->getInt("Nmoments")) + "_" + type + "_" + "moments";
// 	}

// };

// #endif
