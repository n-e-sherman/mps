#ifndef __CALCULATOR_H_
#define __CALCULATOR_H_

#include "itensor/all.h"
#include "util.h"
#include "service.h"

using namespace itensor;

class Calculator : public Service
{

public:

	Calculator() : Service() {}
	~Calculator() {}	
};
#endif