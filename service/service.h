#ifndef __SERVICE_H_
#define __SERVICE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include <vector>
#include <string>

using namespace itensor;

class Service
{
protected:

	Args* args;
	
public:

	Service(Args* a) : args(a) {}
	virtual void calculate() = 0;
	virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
	{	
		auto labels = _labels();
		auto results = _results();
		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
	}
	virtual vector<string> _labels() = 0;
	virtual vector<vector<StringReal>> _results() = 0;


};
#endif