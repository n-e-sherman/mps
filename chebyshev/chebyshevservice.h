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
	ChebyshevService(){}
	ChebyshevService(ChebyshevBuilder* cb, RepositoryBuilder* rb) {	chebyshevBuilder = cb; repoBuilder = rb; }
	~ChebyshevService(){}

	void calculate(Args* args)
	{
		repo = repoBuilder->build(args);
		chebyshev = chebyshevBuilder->build(args);
    	auto save = args->getBool("saveChebyshev");
		int nSave = args->getInt("nSave");
		int nChebyshev = args->getInt("nChebyshev");
		string type = (args->getBool("momentum") ? "p" : "x");
		int remaining = nChebyshev - chebyshev->getIteration();
		while(remaining > 0)
		{
			cout << "remaining: " << remaining << endl;
			chebyshev->calculate();
			remaining = nChebyshev - chebyshev->getIteration();
			repo->save(Chebyshev::getHash(args),chebyshev,save);
			auto [labels,results] = chebyshev->getResults();
			repo->save(Chebyshev::getHash(args),"chebyshev"+type+"/"+args->getString("Model"),labels,results);
		}
		auto [labels,results] = chebyshev->getResults();
		repo->save(Chebyshev::getHash(args),"chebyshev"+type+"/"+args->getString("Model"),labels,results); 
	}
};

#endif
