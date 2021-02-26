#ifndef __OPERATORBUILDER_H_
#define __OPERATORBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "operator/operator.h"
#include "operator/momentum.h"
#include "operator/position.h"
#include "sites/sitesbuilder.h"
#include "state/state.h"
#include "lattice/latticebuilder.h"


using namespace itensor;
using namespace std;

class OperatorBuilder
{
protected:

	SitesBuilder* sitesBuilder;
	LatticeBuilder* latticeBuilder;

public:

	OperatorBuilder(SitesBuilder* sb, LatticeBuilder* lb) :  sitesBuilder(sb), latticeBuilder(lb) {}	
	Operator* build(Args* args_in, std::string key = "")
	{
		auto base = "operator";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto momentum = args->getBool("momentum");
		string _cout = ((momentum) ? "Momentum" : "Position");
		cout << "building operator: " << _cout << " -- key: " << key << endl;
		if(momentum)
		{
			return new Momentum(args,sitesBuilder->build(args, key),latticeBuilder->build(args, key));
		}
		else
		{
			return new Position(args,sitesBuilder->build(args, key));
		}
		
		return nullptr;
	}
};
#endif