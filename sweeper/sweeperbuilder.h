#ifndef __SWEEPERBUILDER_H_
#define __SWEEPERBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "repository/repository.h"
#include "sweeper/sweeper.h"
#include "sweeper/krylov.h"
#include "sweeper/projection.h"
#include "sweeper/identity.h"
#include "sweeper/exact.h"


class SweeperBuilder
{
protected:


public:
	SweeperBuilder(){}
	~SweeperBuilder(){}
	Sweeper* build(Args* args)
	{

		auto sweeperType = args->getString("sweeperType");
		if (sweeperType == "identity")
		{
			return new Identity(args);
		}
		else
		if(sweeperType == "projection")
		{
			return new Projection(args);
		}
		else
		if(sweeperType == "exact")
		{
			return new Exact(args);
		}
		else
		{
			/* Add other sweepers if you implement. */
			return nullptr;
		}

	}
};
#endif