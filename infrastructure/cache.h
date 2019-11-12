#ifndef __CACHE_H_
#define __CACHE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/cache.h"
#include <vector>
#include <string>
#include <map>

/* This is a singleton class */
class Cache
{
private:
	Cache(){}
	std::map<std::string, void*> cache;
	static Cache* instance;
public:
	static Cache* getInstance()
	{
		if(!instance)
			instance = new Cache;
		return instance;
	}

	void save(const std::string& s, void* p)
	{
		cout << "Added " + s + " to cache." << endl;
		cache[s] = p;
	}
	void* load(const std::string& s)
	{ 
		if(cache.find(s) != cache.end()) {cout << "Retrieved " + s + " from cache." << endl; return cache[s]; }
		else return nullptr;
	}

}; Cache* Cache::instance = nullptr;

#endif