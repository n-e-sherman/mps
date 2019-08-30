#ifndef __CACHE_H_
#define __CACHE_H_

#include "itensor/all.h"

class Cache
{
public:
	struct Entry
	{

		void* ptr;
	};
protected:
public:
	Cache(){}
	void add(){}
	void* get(){}
};

#endif