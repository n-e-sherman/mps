#ifndef __CHEBYSHEVSERVICE_H_
#define __CHEBYSHEVSERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "chebyshev/chebyshevbuilder.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace itensor;
using namespace std;

class ChebyshevService
{
private:

	ChebyshevBuilder* chebyshevBuilder;
	Chebyshev* chebyshev;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:

	ChebyshevService(ChebyshevBuilder* cb, RepositoryBuilder* rb) : chebyshevBuilder(cb), repoBuilder(rb) {}
	
	void calculate(Args* args_in)
	{
		auto key =  "chebyshevService";
		auto args = build_args(args_in, key);

		repo = repoBuilder->build(args, key);
		chebyshev = chebyshevBuilder->build(args, key);
    	auto save = args->getBool("save");
    	
		int nChebyshev = args->getInt("nChebyshev");
		string type = (args->getBool("momentum") ? "p" : "x");
		int remaining = nChebyshev - chebyshev->getIteration();
		while(remaining > 0)
		{
			cout << "n = " << chebyshev->getIteration() << " / " << nChebyshev << endl;
			chebyshev->calculate();
			remaining = nChebyshev - chebyshev->getIteration();
			repo->save(Chebyshev::getHash(args),chebyshev,save);
			auto [labels,results] = chebyshev->getResults();
			repo->save(Chebyshev::getHash(args),"chebyshev"+type+"/"+args->getString("Model")+"/"+args->getString("Lattice"),labels,results);
		}
		auto [labels,results] = chebyshev->getResults();
		repo->save(Chebyshev::getHash(args),"chebyshev"+type+"/"+args->getString("Model")+"/"+args->getString("Lattice"),labels,results,true); 
	}
};

#endif
