#ifndef __MEASUREMENTBUILDER_H_
#define __MEASUREMENTBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "measurement/measurement.h"
#include "measurement/realspace.h"
#include "measurement/kspace.h"
#include "measurement/centersite.h"
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
	Measurement* build(Args* args_in, std::string key = "")
	{
		auto base = "measurement";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto momentum = args->getBool("momentum");
		auto thermal = args->getBool("thermal");
		auto centerSite = args->getBool("CenterSite");
		auto CSargs = new Args(*args);
		CSargs->add("momentum",false);
		cout << "building measurement: ";

		if(momentum)
		{
			if(centerSite)
			{
				cout << "CenterSite" << " -- key: " << key << endl; 
				return new CenterSite(args, operatorBuilder->build(CSargs, key));
			}
			cout << "KSpace" << " -- key: " << key << endl; 
			return new KSpace(args, operatorBuilder->build(args, key));

		}
		else
		{
			if(thermal)
			{
				cout << "RealSpace" << " -- key: " << key << endl;  
				return new RealSpace(args, latticeBuilder->build(args, key), sitesBuilder->build(args, key)); 	
			}
			cout << "Local" << " -- key: " << key << endl;  
			return new Local(args, sitesBuilder->build(args, key));
		}
		
		
		
	}
};
#endif