#ifndef __MEASUREMENTBUILDER_H_
#define __MEASUREMENTBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "measurement/measurement.h"
#include "measurement/realspace.h"
#include "measurement/kspace.h"
#include "measurement/local.h"
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
		auto thermal = args->getBool("thermal");
		cout << "building measurement: ";
		if (thermal)
		{
			if (momentum) { cout << "KSpace" << endl; return new KSpace(args, operatorBuilder->build(args)); }
			else { cout << "RealSpace" << endl; return new RealSpace(args, latticeBuilder->build(args), sitesBuilder->build(args)); }
		}
		else
		{
			cout << "Local" << endl;
			return new Local(args, sitesBuilder->build(args));
		}
		
		
	}
};
#endif