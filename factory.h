#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"
#include "latticebuilder.h"
#include "modelbuilder.h"
#include "sitebuilder.h"
#include "groundstatecalculator.h"
#include "repositorybuilder.h"

/* Memory manage the new pointers. */

class Factory
{

private:
	InputGroup* input;	
	LatticeBuilder* latticeFactory() {return new LatticeBuilder(); }
	ModelBuilder*   modelFactory()   {return new ModelBuilder(latticeFactory(), siteFactory()); }
	SiteBuilder*    siteFactory()    {return new SiteBuilder(); }

	GroundStateCalculator* groundStateFactory() {return new GroundStateCalculator(modelFactory(), siteFactory()); }
	RepositoryBuilder* repositoryFactor(){return new RepositoryBuilder(); }


public:

	Factory(){}
	Factory(InputGroup* i) {input = i;}
	~Factory(){}
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
