#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_
#include "infrastructure/builder.h"
#include "repo/repository.h"


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
	}
};
#endif