#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_

#include "repository/repository.h"


class RepositoryBuilder
{
public:
	RepositoryBuilder() {}
	~RepositoryBuilder(){}
	Repository* build(Args* args)
	{
		auto cwd = args->getString("cwd");
		auto readFile = args->getBool("readFile");
		return new Repository(cwd,readFile);
	}
};
#endif