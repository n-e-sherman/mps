#ifndef __SITESBUILDER_H_
#define __SITESBUILDER_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "sites/sites.h"
#include <string>

using namespace std;

class SitesBuilder
{
protected:

public:

	Sites* build(Args* args_in, std::string key = "")
	{
		auto base = "sites";
		key = key + "." + base;
		auto args = build_args(args_in, base, key);
		
		auto cache = Cache::getInstance();
		auto sites = (Sites*)cache->load(Sites::getHash(args));
		if (sites) return sites; // in the cache
		auto N = args->getInt("N"); if(args->getBool("thermal")) N = 2*N;
		auto sType = args->getString("SiteSet");
		cout << "building sites: " << sType << " -- key: " << key << endl;
		if (sType == "SpinHalf")
		{
			sites = new Sites(args,SpinHalf(N,*args));			
		}
		else
		if (sType == "SpinOne")
		{
			sites = new Sites(args,SpinOne(N,*args));	
		}
		else
		if (sType == "SpinTwo")
		{
			sites = new Sites(args,SpinTwo(N,*args));
		}
		else
		{
			sites = new Sites(args,SiteSet(N));	
		}
		cache->save(Sites::getHash(args),sites);
		return sites;
	}
};
#endif