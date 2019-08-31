#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_
#include "builder.h"
#include "filesystem.h"
#include "repository.h"


class RepositoryBuilder : public Builder
{
protected:
	// LatticeBuilder* latticeBuilder;
	// SiteBuilder* siteBuilder;
	// SiteSet sites;
	// Lattice* lattice;
public:
	RepositoryBuilder() {}
	~RepositoryBuilder(){}
	Repository* build()
	{
		return new Repository();
		// TODO: You have no validator that repo will be created.
	}
};
#endif