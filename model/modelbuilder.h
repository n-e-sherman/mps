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
public:
	enum modelType
	{
		normal = 0,
		thermal = 1
	};
protected:
	LatticeBuilder* latticeBuilder;
	SiteBuilder* siteBuilder;
	SiteSet sites;
	Lattice* lattice;
public:
	ModelBuilder() {}
	ModelBuilder(LatticeBuilder* lb, SiteBuilder* sb) { latticeBuilder = lb; siteBuilder = sb; }
	~ModelBuilder(){}
	Model* build(InputGroup* input, modelType mType = normal) // Make a general argument system and pass that and put the enum in it
	{
		sites = siteBuilder->build(input);
		lattice = latticeBuilder->build(input);
		auto model = input->getString("model","Heisenberg");
		auto thermal = input->getYesNo("thermal",0);
		Model* M;
		if(model == "Heisenberg") 
		{
			if(mType == normal) M = new Heisenberg(lattice, sites, input);
			else if(mType == thermal) M = new HeisenbergLouiville(lattice, sites, input);
		}

		// Fill in other models as they come up.
		// else if(model == "")
		return M;
		// TODO: You have no validator that M will be created.
	}
};
#endif