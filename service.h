#ifndef __SERVICE_H_
#define __SERVICE_H_

#include "itensor/all.h"

using namespace itensor;

class Service
{
protected:
	Args args;
public:

	Service() {}
	Service(Args a) : args(a) {}
	virtual void calc() = 0;

};
#endif