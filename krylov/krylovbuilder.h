#ifndef __KRYLOVBUILDER_H_
#define __KRYLOVBUILDER_H_
#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"
#include "infrastructure/builder.h"
#include "lattice/thermalchain.h"
#include "lattice/chain.h"
#include "infrastructure/cache.h"

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
	modelBuilder* modelBuilder;
	Model* model;
	StateBuilder* stateBuilder;
	State* state;
public:
	KrylovBuilder(ModelBuilder* mb, StateBuilder* sb){ modelBuilder = mb; stateBuilder = sb; }
	~KrylovBuilder(){}
	Krylov* build(Args* a, KrylovType kType = reorthogonalize)
	{
		args = a;
		auto spectral = args->getYesNo("spectral",1);
		if(spectral) state = stateBuilder->build(args,StateBuilder::spectral);
		else
		{
			/* If you want anything else */
		}
		auto thermal = args->getYesNo("thermal",0);
		if(thermal) model = modelBuilder->build(args,ModelBuilder::thermal);
		else model = modelBuilder->build(args);
		auto reortho = args->getYesNo("reorthogonalize",1);
		if(kType == reorthogonalize) return new Reorthogonalize(args,model,state);
		else return new Krylov(args,model,state);
	}


		// TODO: You have no validator to make sure sites was created.
	}
};
#endif