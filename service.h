#ifndef __SERVICE_H_
#define __SERVICE_H_

#include "itensor/all.h"
#include "util.h"

using namespace itensor;

class Service
{
protected:
	Args* args;
public:

	Service() {}
	~Service() {}
	/* Maybe include build, calculate, validate */
};
#endif