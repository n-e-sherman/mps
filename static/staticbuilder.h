#ifndef __STATICBUILDER_H_
#define __STATICBUILDER_H_


#include "itensor/all.h"
#include "infrastructure/util.h"
#include "static/static.h"
#include "static/magnetization.h"
#include "static/staticcorrelation.h"
#include "state/statebuilder.h"
#include "operator/operator.h"
#include "measurement/measurementbuilder.h"
#include "repository/repositorybuilder.h"



using namespace itensor;
using namespace std;

class StaticBuilder
{
protected:

	StateBuilder* stateBuilder;
	MeasurementBuilder* measurementBuilder;
	OperatorBuilder* operatorBuilder;
	LatticeBuilder* latticeBuilder;
	RepositoryBuilder* repoBuilder; // Not currently in use.

public:

	StaticBuilder(StateBuilder* sb, MeasurementBuilder * msb, OperatorBuilder* ob, LatticeBuilder* lb, RepositoryBuilder* rb) : stateBuilder(sb), measurementBuilder(msb), operatorBuilder(ob), latticeBuilder(lb), repoBuilder(rb) {}
	Static* build(Args* args_in, std::string key = "")
	{
		auto base = "static";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		cout << "building static: -- key: " << key << endl;


		auto staticType = args->getString("Static");
		if(staticType == "Magnetization")
		{
			return new Magnetization(args, stateBuilder, measurementBuilder);	
		}
		else
		if(staticType == "StaticCorrelation")
		{
			auto repo = repoBuilder->build(args, key);
			auto load = args->getBool("load");
			auto xargs = new Args(*args);
			auto pargs = new Args(*args);
			xargs->add("momentum",false);
			pargs->add("momentum",true);
			// StaticCorrelation(Args* a, Lattice* l; Measurement* ms, State* s, Operator* ox, Operator* oq)
			auto correlation = repo->load(StaticCorrelation::getHash(args), new StaticCorrelation(args, latticeBuilder->build(args), measurementBuilder->build(xargs), measurementBuilder->build(pargs), operatorBuilder->build(xargs), operatorBuilder->build(pargs)));
			if((correlation != nullptr) && load) return correlation;
			correlation = new StaticCorrelation(args, latticeBuilder->build(args), measurementBuilder->build(xargs), measurementBuilder->build(pargs), stateBuilder->build(args), operatorBuilder->build(xargs), operatorBuilder->build(pargs));
			repo->save(StaticCorrelation::getHash(args), correlation);
			return correlation;
		}
		else
		{
			/* Implement other statics here, may want to use else if */
			return nullptr;
		}
	}
};
#endif