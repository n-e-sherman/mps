#ifndef __MEASUREMENTBUILDER_H_
#define __MEASUREMENTBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "measurement/measurement.h"
#include "measurement/realspace.h"
#include "measurement/kspace.h"
#include "sites/sitesbuilder.h"
#include "lattice/latticebuilder.h"
#include "operator/operatorbuilder.h"

using namespace itensor;
using namespace std;

class MeasurementBuilder
{
protected:

	SitesBuilder* sitesBuilder;
	LatticeBuilder* latticeBuilder;
	OperatorBuilder* operatorBuilder;

public:

	MeasurementBuilder(SitesBuilder* sb, LatticeBuilder* lb, OperatorBuilder* ob) : 
					   sitesBuilder(sb), latticeBuilder(lb), operatorBuilder(ob) {}
	Measurement* build(Args* args)
	{
		auto momentum = args->getBool("momentum");
		string _cout = ((momentum) ? "KSpace" : "RealSpace");
		cout << "building measurement: " << _cout << endl;
		if (momentum)
			return new KSpace(args, operatorBuilder->build(args));
		return new RealSpace(args, latticeBuilder->build(args), sitesBuilder->build(args));
	}
};
#endif