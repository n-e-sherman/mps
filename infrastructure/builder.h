#ifndef __BUILDER_H_
#define __BUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/service.h"

using namespace itensor;

class Builder : public Service
{

public:

	Builder() : Service() {}
	virtual ~Builder() {}
};
#endif