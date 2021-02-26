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
	Chebyshev* build(Args* args_in, std::string key = "")
	{
		auto base = "chebyshev";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);

		cout << "building chebyshev -- key: " << key << endl;
		repo = repoBuilder->build(args, key);
		auto load = args->getBool("load"); 

		auto chebyshev = repo->load(Chebyshev::getHash(args), new Chebyshev(args, measurementBuilder->build(args, key), modelBuilder->build(args, key)));
		if((chebyshev != nullptr) && load) return chebyshev;
		chebyshev = new Chebyshev(args, measurementBuilder->build(args, key), modelBuilder->build(args, key), stateBuilder->build(args, key), operatorBuilder->build(args, key));
		repo->save(Chebyshev::getHash(args), chebyshev);
		return chebyshev;
	}
};
#endif