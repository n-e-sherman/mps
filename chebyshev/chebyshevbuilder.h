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


class ChebyshevBuilder
{
protected:
	ModelBuilder* modelBuilder;
	StateBuilder* stateBuilder;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:
	ChebyshevBuilder(){}
	ChebyshevBuilder(ModelBuilder* mb, StateBuilder* sb, LatticeBuilder* lb, RepositoryBuilder* rb)
	{
		modelBuilder = mb;
		stateBuilder = sb;
		latticeBuilder = lb;
		repoBuilder = rb;
	}
	~ChebyshevBuilder(){}
	Chebyshev* build(Args* args)
	{
		auto readFile = args->getBool("cheReadFile");
		auto writeFile = args->getBool("cheWriteFile");
		repo = repoBuilder->build(args,readFile,writeFile);
		auto momentum = args->getBool("momentum");
		auto thermal = args->getBool("thermal");
		auto save = args->getBool("saveChebyshev");
		if(momentum) 
		{
				auto chebyshev = repo->load(Chebyshev::getHash(args), new Chebyshevp(args, modelBuilder->build(args)));
				if(chebyshev != nullptr) return chebyshev;
				if(thermal) chebyshev = new Chebyshevp(args, modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));
				else {chebyshev = new Chebyshevp(args, modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));}
				repo->save(Chebyshev::getHash(args), chebyshev,save);
				return chebyshev;
		}
		else
		{
				auto chebyshev = repo->load(Chebyshev::getHash(args), new Chebyshevx(args, modelBuilder->build(args)));
				if(chebyshev != nullptr) return chebyshev;
				if(thermal) chebyshev = new Chebyshevx(args, modelBuilder->build(args),stateBuilder->build(args,StateBuilder::thermal),latticeBuilder->build(args));
				else {chebyshev = new Chebyshevx(args, modelBuilder->build(args),stateBuilder->build(args,StateBuilder::ground),latticeBuilder->build(args));}
				repo->save(Chebyshev::getHash(args), chebyshev,save);
				return chebyshev;
		}
	}
};
#endif