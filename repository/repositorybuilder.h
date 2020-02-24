#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_

#include "repository/repository.h"


class RepositoryBuilder
{
public:
	RepositoryBuilder() {}
	~RepositoryBuilder(){}
	Repository* build(Args* args, bool rf = false, bool wf = false)
	{
		auto dataDir = args->getString("dataDir");
		auto resDir = args->getString("resDir");
		return new Repository(dataDir,resDir,rf, wf);
	}
};
#endif