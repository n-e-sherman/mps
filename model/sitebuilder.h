#ifndef __SITEBUILDER_H_
#define __SITEBUILDER_H_

#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"
#include "infrastructure/cache.h"
#include <string>

using namespace std;

class SiteBuilder
{
protected:
	SiteSet sites;
public:
	SiteBuilder(){}
	~SiteBuilder(){}
	SiteSet build(Args* args)
	{
		cout << "building sites" << endl;
		auto sType = args->getString("SiteSet");
		auto cache = Cache::getInstance();
		auto N = args->getInt("N");
		if(args->getBool("thermal")) N = 2*N;
		auto pSites = (SiteSet*)cache->load(SiteBuilder::getHash(args));
		if(pSites != nullptr) 
		{
			if     (sType == "SpinHalf"){ auto res = (SpinHalf*)cache->load(SiteBuilder::getHash(args)); return *res;}
			else if(sType == "SpinOne") { return *(SpinOne*)cache->load(SiteBuilder::getHash(args)); }
			else if(sType == "SpinTwo") { return *(SpinTwo*)cache->load(SiteBuilder::getHash(args)); } 
			/* Add other SiteSet. */
			return *(SiteSet*)cache->load(SiteBuilder::getHash(args));
		}
		if     (sType == "SpinHalf"){ sites = SpinHalf(N); }
		else if(sType == "SpinOne") { sites = SpinOne(N);  }
		else if(sType == "SpinTwo") { sites = SpinTwo(N);  } 
			/* Add other SiteSet. */
		cache->save(SiteBuilder::getHash(args), &sites);
		return sites;
	}

	static string getHash(Args* args)
	{
		return to_string(args->getInt("N")) + "_" + args->getString("SiteSet") + "_" + to_string(args->getBool("thermal"));
	}
};
#endif