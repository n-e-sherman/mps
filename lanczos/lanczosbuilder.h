#ifndef __KRYLOVBUILDER_H_
#define __KRYLOVBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "lanczos/lanczos.h"
#include "lanczos/reorthogonalize.h"

class LanczosBuilder
{
public:
	enum krylovType
	{
		bare = 0,
		reorthogonalize = 1

	};
protected:
	Args* args;
	ModelBuilder* modelBuilder;
	StateBuilder* stateBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;
public:
	LanczosBuilder(ModelBuilder* mb, StateBuilder* sb, RepositoryBuilder* rb){ modelBuilder = mb; stateBuilder = sb; repoBuilder = rb;}
	~LanczosBuilder(){}
	Lanczos* build(Args* a, krylovType kType = reorthogonalize)
	{
		cout << "building krylov" << endl;
		args = a;
		repo = repoBuilder->build(args);
		auto spectral = args->getBool("spectral");
		auto thermal = args->getBool("thermal");
		auto reortho = args->getBool("reorthogonalize");
		if(spectral) 
		{
			if(thermal) 
			{
				if(kType == reorthogonalize) 
				{
					auto lanczos = repo->load(Reorthogonalize::getHash(args), new Reorthogonalize());
					if(lanczos != nullptr) return lanczos;
					lanczos = new Reorthogonalize(args,modelBuilder->build(args,ModelBuilder::thermal),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), lanczos);
					return lanczos;
				}
				else 
				{
					auto lanczos = repo->load(Lanczos::getHash(args), new Lanczos());
					if(lanczos != nullptr) return lanczos;
					lanczos = new Lanczos(args,modelBuilder->build(args,ModelBuilder::thermal),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), lanczos);
					return lanczos;
				}
			}
			else 
			{
				if(kType == reorthogonalize) 
				{
					auto lanczos = repo->load(Reorthogonalize::getHash(args), new Reorthogonalize());
					if(lanczos != nullptr) return lanczos;
					lanczos = new Reorthogonalize(args,modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), lanczos);
					return lanczos;
				}
				else 
				{
					auto lanczos = repo->load(Lanczos::getHash(args), new Lanczos());
					if(lanczos != nullptr) return lanczos;
					lanczos = new Lanczos(args,modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), lanczos);
					return lanczos;
				}
			}
		}
		else
		{
			/* Implement other states here, you may want an else if. */
			return nullptr;
		}
	}
};
#endif