#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_

#include "repository/repository.h"


class RepositoryBuilder
{
public:
	
	Repository* build(Args* args)
	{
		auto dataDir = args->getString("dataDir");
		auto resDir = args->getString("resDir");
		auto rf = args->getBool("read");
		auto wf = args->getBool("write");
		return new Repository(dataDir,resDir,rf, wf);
	}
};
#endif