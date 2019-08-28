#ifndef __BUILDER_H_
#define __BUILDER_H_

#include "itensor/all.h"
#include "util.h"
#include "service.h"

using namespace itensor;

class Builder : public Service
{

public:

	Builder() : Service() {}
	virtual ~Builder() {}
};
#endif