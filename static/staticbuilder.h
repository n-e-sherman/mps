#ifndef __STATICBUILDER_H_
#define __STATICBUILDER_H_


#include "itensor/all.h"
#include "infrastructure/util.h"
#include "static/static.h"
#include "static/magnetization.h"
#include "state/statebuilder.h"
#include "measurement/measurementbuilder.h"
#include "repository/repositorybuilder.h"



using namespace itensor;
using namespace std;

class StaticBuilder
{
protected:

	StateBuilder* stateBuilder;
	MeasurementBuilder* measurementBuilder;
	RepositoryBuilder* repoBuilder; // Not currently in use.

public:

	StaticBuilder(StateBuilder* sb, MeasurementBuilder * msb, RepositoryBuilder* rb) : stateBuilder(sb), measurementBuilder(msb), repoBuilder(rb) {}
	Static* build(Args* args)
	{
		cout << "building static: " << endl;
		return new Magnetization(args, stateBuilder, measurementBuilder);
		/* May want to include other options in the future. */
	}
};
#endif