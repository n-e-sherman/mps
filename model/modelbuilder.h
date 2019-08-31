#ifndef __MODELBUILDER_H_
#define __MODELBUILDER_H_
#include "heisenberg.h"
#include "lattice/latticebuilder.h"
#include "infrastructure/builder.h"
#include "sitebuilder.h"
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
		Model* M;
		if(model == "Heisenberg") M = new Heisenberg(lattice, sites, input);
		return M;
		// TODO: You have no validator that M will be created.
	}
};
#endif