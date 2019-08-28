#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"
#include "latticebuilder.h"
#include "modelbuilder.h"
#include "sitebuilder.h"
#include "groundstatecalculator.h"

class Factory
{

private:
	InputGroup* input;	
	LatticeBuilder* latticeFactory() { return new LatticeBuilder(); }
	ModelBuilder*   modelFactory()   {return new ModelBuilder(latticeFactory(), siteFactory()); }
	SiteBuilder*    siteFactory()    {return new SiteBuilder(); }

	GroundStateCalculator* groundStateFactory() {return new GroundStateCalculator(modelFactory(), siteFactory()); }
public:

	Factory(){}
	Factory(InputGroup* i) {input = i;}
	void run()
	{
		/* TODO: This needs to be general, just for testing.*/
		auto quantity = input->getString("quantity","groundState");
		if(true)
		{
			auto q = groundStateFactory();
			auto [E0,psi] = q->calculate(input);
			Print(E0);
			Print(psi);
		}
	}

};
#endif
