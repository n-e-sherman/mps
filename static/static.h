#ifndef __STATIC_H_
#define __STATIC_H_


#include "service/service.h"
#include "measurement/realspace.h"
#include "state/statebuilder.h"

using namespace itensor;

class Static : public Service
{	
public:

	Static(Args* a) : Service(a) {}



};
#endif