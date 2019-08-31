#ifndef __CACHE_H_
#define __CACHE_H_

#include "itensor/all.h"
#include <vector>
#include <string>
#include <map>

/* This is a singleton class */
class Cache
{
private:
	Cache(){}
	std::map<std::string, void*> cache;
	static Cache* instance = nullptr;
public:
	static Cache* getInstance()
	{
		if(instance == nullptr)
			instance = new Cache();
		return instance;
	}

	void add(std::string& s, void* p)
	{ 
		if(cache.find(s) == cache.end()) cache[s] = p; 
	}
	void* get(std::string& s)
	{ 
		if(cache.find(s) != cache.end()) return cache(s);
		else return nullptr;
	}

};

#endif