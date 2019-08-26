#ifndef __READ_H_
#define __READ_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include <iomanip>
#include "util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Read : public Service
{
// protected:
public:

	Read() {}
	Read(Args a) : Service(a) {}
	virtual void calc() { /* Implement me */};

};
#endif