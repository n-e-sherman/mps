#ifndef __SITEBUILDER_H_
#define __SITEBUILDER_H_
#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"
#include "infrastructure/builder.h"
#include "lattice/thermalchain.h"
#include "lattice/chain.h"
#include "infrastructure/cache.h"

class SiteBuilder
{
protected:
	SiteSet sites;
public:
	SiteBuilder(){}
	~SiteBuilder(){}
	SiteSet build(Args* args)
	{
		auto cache = Cache::getInstance();
		auto sType = args->getString("siteType","spinHalf");
		auto N = args->getInt("N",100);
		auto hash = to_string(N)+sType;
		auto s = (SiteSet*)cache->get(hash);
		if(s != nullptr) {sites = *s; return sites; }
		if     (sType == "spinHalf"){ sites = SpinHalf(N); }
		else if(sType == "spinOne") { sites = SpinOne(N);  }
		else if(sType == "spinTwo") { sites = SpinTwo(N);  } 
		cache->add(hash, &sites);
		return sites;
		// TODO: You have no validator to make sure sites was created.
	}
};
#endif