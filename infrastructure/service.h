#ifndef __SERVICE_H_
#define __SERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"

using namespace itensor;

class Service
{
protected:
	Args* args;
public:

	Service() {}
	virtual ~Service() {}
};
#endif