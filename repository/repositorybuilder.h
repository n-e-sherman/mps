#ifndef __REPOSITORYBUILDER_H_
#define __REPOSITORYBUILDER_H_

#include "repository/repository.h"


class RepositoryBuilder
{
public:
	
	Repository* build(Args* args_in, std::string key = "")
	{
		auto base = "repository";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto dataDir = args->getString("dataDir");
		auto resDir = args->getString("resDir");
		auto rf = args->getBool("read");
		auto wf = args->getBool("write");
		return new Repository(dataDir,resDir,rf, wf);
	}
};
#endif