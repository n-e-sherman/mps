#ifndef __KRYLOVBUILDER_H_
#define __KRYLOVBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repositorybuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "krylov/krylov.h"
#include "krylov/reorthogonalize.h"

class KrylovBuilder
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
	KrylovBuilder(ModelBuilder* mb, StateBuilder* sb, RepositoryBuilder* rb){ modelBuilder = mb; stateBuilder = sb; repoBuilder = rb;}
	~KrylovBuilder(){}
	Krylov* build(Args* a, krylovType kType = reorthogonalize)
	{
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
					auto krylov = repo->load(Reorthogonalize::getHash(args), new Reorthogonalize());
					if(krylov != nullptr) return krylov;
					krylov = new Reorthogonalize(args,modelBuilder->build(args,ModelBuilder::thermal),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), krylov);
					return krylov;
				}
				else 
				{
					auto krylov = repo->load(Krylov::getHash(args), new Krylov());
					if(krylov != nullptr) return krylov;
					krylov = new Krylov(args,modelBuilder->build(args,ModelBuilder::thermal),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), krylov);
					return krylov;
				}
			}
			else 
			{
				if(kType == reorthogonalize) 
				{
					auto krylov = repo->load(Reorthogonalize::getHash(args), new Reorthogonalize());
					if(krylov != nullptr) return krylov;
					krylov = new Reorthogonalize(args,modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), krylov);
					return krylov;
				}
				else 
				{
					auto krylov = repo->load(Krylov::getHash(args), new Krylov());
					if(krylov != nullptr) return krylov;
					krylov = new Krylov(args,modelBuilder->build(args),stateBuilder->build(args,StateBuilder::spectral));
					repo->save(Reorthogonalize::getHash(args), krylov);
					return krylov;
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