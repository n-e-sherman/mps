#ifndef __MEASUREMENTBUILDER_H_
#define __MEASUREMENTBUILDER_H_

/* This needs some serious love, deal with this later */

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "operator/operatorbuilder.h"
#include "measurement/measurement.h"
#include "measurement/connectedx.h"
#include "measurement/connectedp.h"

using namespace itensor;
using namespace std;

class MeasurementBuilder
{
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	LatticeBuilder* latticeBuilder;
	OperatorBuilder* operatorBuilder;
	RepositoryBuilder* repoBuilder;
	
	Repository* repo;

public:

	MeasurementBuilder(ModelBuilder* mb, LatticeBuilder* lb, OperatorBuilder* ob, RepositoryBuilder* rb) : 
					   modelBuilder(mb), latticeBuilder(lb), operatorBuilder(ob), repoBuilder(rb) {}
	Measurement* build(Args* a)
	{
		cout << "building operator" << endl;
		args = a;
		repo = repoBuilder->build(args);
		auto m = args->getString("Measurement");
		if(m == "Connected")
		{
			auto momentum = args->getBool("momentum");
			if (momentum)
			{
				return new Connectedp(args,modelBuilder->build(args),operatorBuilder->build(args));
			}
			else
			{
				return new Connectedx(args,modelBuilder->build(args),latticeBuilder->build(args));
			}
		}
		else
		{
			/* Implement other operators here, may want to use else if. */
			return nullptr;
		}

	}
};
#endif