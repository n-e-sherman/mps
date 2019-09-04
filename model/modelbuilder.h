#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_
#include "model/heisenberg.h"
#include "lattice/latticebuilder.h"
#include "infrastructure/builder.h"
#include "model/sitebuilder.h"
#include "model/heisenberglouiville.h"
/* Add lattice types in include. */


class ModelBuilder : public Builder
{
protected:
	LatticeBuilder* latticeBuilder;
	SiteBuilder* siteBuilder;
	SiteSet sites;
	Lattice* lattice;
public:
	ModelBuilder() {}
	ModelBuilder(LatticeBuilder* lb, SiteBuilder* sb) { latticeBuilder = lb; siteBuilder = sb; }
	~ModelBuilder(){}
	Model* build(InputGroup* input)
	{
		sites = siteBuilder->build(input);
		lattice = latticeBuilder->build(input);
		auto model = input->getString("model","Heisenberg");
		auto thermal = input->getYesNo("thermal",0);
		Model* M;
		/* This logic needs to get Louiville when thermal and spectral. */
		if(model == "Heisenberg") 
		{
			if(thermal) M = new HeisenbergLouiville(lattice,site,inputs);
			else M = new Heisenberg(lattice, sites, input);
		}

		// Fill in other models as they come up.
		// else if(model == "")
		return M;
		// TODO: You have no validator that M will be created.
	}
};
#endif