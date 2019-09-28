#ifndef __SITEBUILDER_H_
#define __SITEBUILDER_H_
#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"
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
		auto cache = Cache::getInstance();
		auto pSites = (SiteSet*)cache->load(SiteBuilder::getHash(args));
		if(pSites != nullptr) return *pSites;
		auto N = args->getInt("N");
		if(args->getBool("thermal")) N = 2*N;
		auto sType = args->getString("SiteSet");
		if     (sType == "SpinHalf"){ sites = SpinHalf(N); }
		else if(sType == "SpinOne") { sites = SpinOne(N);  }
		else if(sType == "SpinTwo") { sites = SpinTwo(N);  } 
		cache->save(SiteBuilder::getHash(args), &sites);
		return sites;
	}

	static string getHash(Args* args)
	{
		return to_string(args->getInt("N")) + "_" + args->getString("SiteSet") + "_" + to_string(args->getBool("thermal"));
	}
};
#endif