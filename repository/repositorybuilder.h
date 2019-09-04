#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_
#include "infrastructure/builder.h"
#include "repository/repository.h"


class RepositoryBuilder : public Builder
{
public:
	RepositoryBuilder() {}
	~RepositoryBuilder(){}
	Repository* build()
	{
		return new Repository();
	}
};
#endif