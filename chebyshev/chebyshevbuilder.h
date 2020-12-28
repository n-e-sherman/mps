#ifndef __CHEBYSHEVBUILDER_H_
#define __CHEBYSHEVBUILDER_H_

#include "itensor/all.h"
#include "itensor/util/print_macro.h"
#include "infrastructure/util.h"
#include "chebyshev/chebyshev.h"
#include "measurement/measurementbuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "operator/operatorbuilder.h"
#include "repository/repositorybuilder.h"

class ChebyshevBuilder
{
protected:

	MeasurementBuilder* measurementBuilder;
	ModelBuilder* modelBuilder;
	StateBuilder* stateBuilder;
	OperatorBuilder* operatorBuilder;
	RepositoryBuilder* repoBuilder;
	Repository* repo;

public:
	ChebyshevBuilder(MeasurementBuilder* msb, ModelBuilder* mb, StateBuilder* sb, OperatorBuilder* ob, RepositoryBuilder* rb) :
					 measurementBuilder(msb), modelBuilder(mb), stateBuilder(sb), operatorBuilder(ob), repoBuilder(rb) {}
	Chebyshev* build(Args* args)
	{
		cout << "building chebyshev" << endl;
		repo = repoBuilder->build(args);
		
		auto chebyshev = repo->load(Chebyshev::getHash(args), new Chebyshev(args, measurementBuilder->build(args), modelBuilder->build(args)));
		if(chebyshev != nullptr) return chebyshev;
		chebyshev = new Chebyshev(args, measurementBuilder->build(args), modelBuilder->build(args), stateBuilder->build(args), operatorBuilder->build(args));
		repo->save(Chebyshev::getHash(args), chebyshev);
		return chebyshev;
	}
};
#endif