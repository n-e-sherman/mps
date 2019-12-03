#include "itensor/all.h"
#include "infrastructure/factory.h"
#include <iostream>
#include <cstdlib>
#include "infrastructure/util.h"
#include "infrastructure/factory.h"

using namespace itensor;
using namespace std;

int 
main(int argc, char** argv)
{
	Args* args = getArgs(argc,argv);
	auto s1 = spectralWeightsFactory();
	auto s2 = spectralBroadeningFactory();
	auto c  = chebyshevServiceFactory();
	auto m  = momentsFactory();
	if(args->getBool("Chebyshev"))
		c->calculate(args);
	if(args->getBool("Weights"))
		s1->calculate(args);
	if(args->getBool("Broadening"))
		s2->calculate(args);
	if(args->getBool("Moments"))
		m->calculate(args);

}


