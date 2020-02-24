#ifndef __CHEBYSHEVSERVICE_H_
#define __CHEBYSHEVSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "services/service.h"
#include "repository/repositorybuilder.h"
#include "chebyshev/chebyshevbuilder.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace itensor;
using namespace std;

class ChebyshevService : public Service
{
private:
	/* Inputs */
	Args* args;
	ChebyshevBuilder* chebyshevBuilder;
	Chebyshev* chebyshev;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

	/* Outputs */
	vector<Real> res;
	vector<string> labels;
	vector<vector<StringReal>> results;



public:
	ChebyshevService() : Service(){}
	ChebyshevService(ChebyshevBuilder* cb, RepositoryBuilder* rb) : Service()
	{ 
		chebyshevBuilder = cb;
		repoBuilder = rb;
	}
	
	~ChebyshevService(){}

	void calculate(Args* a)
	{
		args = a;

		auto readFile = args->getBool("cheReadFile");
		auto writeFile = args->getBool("cheWriteFile");
		repo = repoBuilder->build(args,readFile,writeFile);
		chebyshev = chebyshevBuilder->build(args);
    	auto save = args->getBool("saveChebyshev");
		int nSave = args->getInt("nSave");
		int nChebyshev = args->getInt("nChebyshev");
		int remaining = nChebyshev - chebyshev->getIteration();
		while(remaining > 0)
		{
			cout << "remaining: " << remaining << endl;
			chebyshev->calculate(min(remaining,nSave));
			remaining = nChebyshev - chebyshev->getIteration();
			repo->save(Chebyshev::getHash(args),chebyshev,save);
		}
		auto [labels,results] = chebyshev->getResults();
		string type = "x";
		if(args->getBool("momentum")) type = "p";
		repo->save(Chebyshev::getHash(args),"chebyshev"+type+"/"+args->getString("Model"),labels,results);
	}





	void processResults()
	{
		/* res in formalt [moment], want to add settings to labels and results */
		auto mom = args->defined("qFactor");
		labels.push_back("moment");
		if(mom) labels.push_back("qFactor");
		else labels.push_back("position");
		labels.push_back("nChebyshev");
		labels.push_back("MaxDim");
		labels.push_back("N");
		labels.push_back("Lattice");
		labels.push_back("Model");
		labels.push_back("thermal");
		labels.push_back("W");
		labels.push_back("Wp");
		labels.push_back("Method");
		if(args->getString("Method")=="Fit") labels.push_back("Nsweep");
		labels.push_back("squared");

		for(auto i : range(res.size()))
		{
			auto temp = vector<StringReal>();
			temp.push_back(res[i]);
			if(mom) {temp.push_back(args->getReal("qFactor")); }
			else {temp.push_back(args->getReal("position")); }
			temp.push_back(args->getReal("nChebyshev"));
			temp.push_back(args->getReal("MaxDim"));
			temp.push_back(args->getReal("N"));
			temp.push_back(args->getString("Lattice"));
			temp.push_back(args->getString("Model"));
			temp.push_back(args->getBool("thermal"));
			temp.push_back(args->getReal("W"));
			temp.push_back(args->getReal("Wp"));
			temp.push_back(args->getString("Method"));
			if(args->getString("Method")=="Fit") temp.push_back(args->getReal("Nsweep"));
			temp.push_back(args->getBool("squared"));
			results.push_back(temp);
		}
	}
};

#endif
