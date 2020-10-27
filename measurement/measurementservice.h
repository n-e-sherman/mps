// #ifndef __MEASUREMENTSERVICE_H_
// #define __MEASUREMENTSERVICE_H_

// #include "itensor/all.h"
// #include "infrastructure/util.h"
// #include "services/service.h"
// #include "repository/repositorybuilder.h"
// #include "measurement/measurementbuilder.h"
// #include <string>
// #include <iostream>
// #include <algorithm>

// using namespace itensor;
// using namespace std;

// class MeasurementService : public Service
// {
// private:
// 	/* Inputs */
// 	Args* args;
// 	MeasurementBuilder* measurementBuilder;
// 	Measurement* measurement;
// 	RepositoryBuilder* repoBuilder;
// 	Repository* repo;

// public:
// 	MeasurementService() : Service(){}
// 	MeasurementService(MeasurementBuilder* mb, StateBuilder*sb,RepositoryBuilder* rb): measurementBuilder = mb;	repoBuilder = rb; }
	
// 	~MeasurementService(){}
// 	void calculate(Args* a)
// 	{
// 		args = a;
// 		repo = repoBuilder->build(args);
// 		measurement = measurementBuilder->build(args);
// 		measurement->calculate();
// 		auto [labels,results] = measurement->getResults();
// 		string type = (args->getBool("momentum") ? "p" : "x");
// 		repo->save(Measurement::getHash(args),"connected"+type+"/"+args->getString("Model"),labels,results);
// 	}
// };

// #endif
