// #ifndef __CHEBYSHEV_H_
// #define __CHEBYSHEV_H_

// #include "itensor/all.h"
// #include "infrastructure/util.h"
// #include "services/service.h"
// #include "model/modelbuilder.h"
// #include "repository/repositorybuilder.h"
// #include "state/statebuilder.h"
// #include <string>
// #include <iostream>

// using namespace itensor;
// using namespace std;

// class Chebyshev : public Service
// {
// private:
// 	/* Inputs */
// 	Args* args;
// 	ModelBuilder* modelBuilder;
// 	Model* model;
// 	StateBuilder* stateBuilder;
// 	State* state;
// 	RepositoryBuilder* repoBuilder;
// 	Repository* repo;
// 	MPO H;

// 	/* Helpers */
// 	MPS psi;
// 	MPS t0;
// 	MPS t1;
// 	MPS t2;
// 	IndexSet is;
// 	vector<Real> res;
// 	int iStart = 2;

// 	/* Outputs */
// 	vector<string> labels;
// 	vector<vector<StringReal>> results;



// public:
// 	Chebyshev() : Service(){}
// 	Chebyshev(ModelBuilder *mb, StateBuilder *sb, RepositoryBuilder* rb) : Service()
// 	{ 
// 		modelBuilder = mb;
// 		repoBuilder = rb;
// 		stateBuilder = sb;
// 	}
	
// 	~Chebyshev(){}

// 	void calculate(Args* a)
// 	{
// 		args = a;
// 		auto thermal = args->getBool("thermal");
// 		auto nChebyshev = args->getInt("nChebyshev");
// 		auto W = args->getReal("W");
// 		if(args->getBool("squared")) W = W*W;
// 		auto Wp = args->getReal("Wp");
// 		auto r = args->getBool("reorthogonalize");
// 		auto nSave = args->getInt("nSave");

// 		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
// 		else model = modelBuilder->build(args);
// 		H = model->getH();
// 		if(thermal) H *= (2.0*Wp/W); /* Only works for thermal, need different scaling for ground */
// 		repo = repoBuilder->build(args);
// 		if(repo->load(getHash(),this) != nullptr) 
// 		{ /* Loading. */
// 			psi.position(1);
// 			t0.position(1);
// 			t1.position(1);
// 			t2.position(1);
// 		}   
// 		else
// 		{ /* Starting a new run. */
// 			state = stateBuilder->build(args,StateBuilder::spectral); /* May want to make this general, might have another thing in mind for the initial state. */
// 			psi = state->getState();
// 			psi.position(1);
// 			t0 = MPS(psi);
// 			res.push_back(inner(t0,t0)); // May want to change inner to innerC if you get complex values at some point. 
// 			t1 = noPrime(applyMPO(H,t0,*args));
// 			t2 = t1;
// 			res.push_back(inner(t0,t1));
// 		}
// 		H.position(1);
// 		is = siteInds(t0);
// 		for(auto i : range(iStart,nChebyshev))
// 		{
// 			cout << i << endl;
// 			auto temp = applyMPO(H,t1,*args);
// 			temp *= 2;
// 			prepare(temp,t0,is);
// 			t2 = sum(temp,-1*t0,*args);
// 			res.push_back(inner(psi,t2));
// 			t0 = t1;
// 			t1 = t2;
// 			iStart = i+1;
// 			if((i % nSave) == 0) repo->save(getHash(),this);
// 		}
// 		processResults();
// 		repo->save(getHash(),this);
// 		repo->save(getHash(),"chebyshev",labels,results);

// 	}
// 	void processResults()
// 	{
// 		/* res in formalt [moment], want to add settings to labels and results */
// 		auto mom = args->defined("qFactor");
// 		labels.push_back("moment");
// 		if(mom) labels.push_back("qFactor");
// 		else labels.push_back("position");
// 		labels.push_back("nChebyshev");
// 		labels.push_back("MaxDim");
// 		labels.push_back("N");
// 		labels.push_back("Lattice");
// 		labels.push_back("Model");
// 		labels.push_back("thermal");
// 		labels.push_back("W");
// 		labels.push_back("Wp");
// 		labels.push_back("Method");
// 		if(args->getString("Method")=="Fit") labels.push_back("Nsweep");
// 		labels.push_back("squared");

// 		for(auto i : range(res.size()))
// 		{
// 			auto temp = vector<StringReal>();
// 			temp.push_back(res[i]);
// 			if(mom) {temp.push_back(args->getReal("qFactor")); }
// 			else {temp.push_back(args->getReal("position")); }
// 			temp.push_back(args->getReal("nChebyshev"));
// 			temp.push_back(args->getReal("MaxDim"));
// 			temp.push_back(args->getReal("N"));
// 			temp.push_back(args->getString("Lattice"));
// 			temp.push_back(args->getString("Model"));
// 			temp.push_back(args->getBool("thermal"));
// 			temp.push_back(args->getReal("W"));
// 			temp.push_back(args->getReal("Wp"));
// 			temp.push_back(args->getString("Method"));
// 			if(args->getString("Method")=="Fit") temp.push_back(args->getReal("Nsweep"));
// 			temp.push_back(args->getBool("squared"));
// 			results.push_back(temp);
// 		}

// 	}


// 	virtual string getHash()
// 	{
// 		string sweeps = "";
// 		if(args->getString("Method") == "Fit") sweeps = "_" + to_string(args->getReal("Nsweeps"));
// 		return State::getHash(args) + "_" + to_string(args->getReal("W")) + "_" + args->getString("Method") + sweeps + "_Chebyshev";
// 	}

// 	virtual void load(ifstream & f)
// 	{

// 		read(f,psi);
// 		read(f,t0);
// 		read(f,t1);
// 		read(f,t2);
// 		read(f,res);
// 		read(f,iStart);

// 	}
// 	virtual void save(ofstream & f)
// 	{
// 		write(f,psi);
// 		write(f,t0);
// 		write(f,t1);
// 		write(f,t2);
// 		write(f,res);
// 		write(f,iStart);
// 	}

// };

// #endif
