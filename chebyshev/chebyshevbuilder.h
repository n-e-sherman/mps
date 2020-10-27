#ifndef __CHEBYSHEVBUILDER_H_
#define __CHEBYSHEVBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "chebyshev/chebyshev.h"
#include "chebyshev/chebyshevx.h"
#include "chebyshev/chebyshevp.h"
#include "model/modelbuilder.h"
#include "repository/repository.h"
#include "lattice/latticebuilder.h"
#include "state/statebuilder.h"
#include "sweeper/sweeperbuilder.h"
#include "measurement/measurementbuilder.h"
#include "operator/operatorbuilder.h"


class ChebyshevBuilder
{
protected:

	ModelBuilder* modelBuilder;
	StateBuilder* stateBuilder;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder;
	SweeperBuilder* sweeperBuilder;
	MeasurementBuilder* measurementBuilder;
	OperatorBuilder* operatorBuilder;
	Repository* repo;

public:

	ChebyshevBuilder(ModelBuilder* mb, StateBuilder* sb, LatticeBuilder* lb, RepositoryBuilder* rb, SweeperBuilder* swpb, MeasurementBuilder* msb, OperatorBuilder* ob) :
					 modelBuilder(mb), stateBuilder(sb), latticeBuilder(lb), repoBuilder(rb),       sweeperBuilder(swpb), measurementBuilder(msb), operatorBuilder(ob) {}
	Chebyshev* build(Args* args)
	{
		repo = repoBuilder->build(args);

		auto momentum = args->getBool("momentum");
		auto thermal = args->getBool("thermal");
		auto save = args->getBool("saveChebyshev");
		auto load = args->getBool("loadChebyshev");
		if(momentum) 
		{
				auto chebyshev = repo->load(Chebyshev::getHash(args), 
					new Chebyshevp(args, 
								   modelBuilder->build(args), 
								   sweeperBuilder->build(args)), load);
				if(chebyshev != nullptr) return chebyshev;
				chebyshev = new Chebyshevp(args, 
										   modelBuilder->build(args),
										   stateBuilder->build(args), 
										   sweeperBuilder->build(args),
										   operatorBuilder->build(args));
				repo->save(Chebyshev::getHash(args), chebyshev, save);
				return chebyshev;
		}
		else
		{
				auto chebyshev = repo->load(Chebyshev::getHash(args), 
					new Chebyshevx(args, 
								   modelBuilder->build(args), 
								   latticeBuilder->build(args), 
								   sweeperBuilder->build(args),
								   measurementBuilder->build(args), 
								   repo), load);
				if(chebyshev != nullptr) return chebyshev;
				chebyshev = new Chebyshevx(args, 
										   modelBuilder->build(args),
										   stateBuilder->build(args),
										   latticeBuilder->build(args), 
										   sweeperBuilder->build(args), 
										   measurementBuilder->build(args), 
										   repo);
				repo->save(Chebyshev::getHash(args), chebyshev, save);
				return chebyshev;
		}
	}
};
#endif