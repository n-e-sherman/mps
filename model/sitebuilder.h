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
	{ /* Needs love */
		auto cache = Cache::getInstance();
		auto sType = args->getString("SiteSet");
		if     (sType == "spinHalf"){ sites = SpinHalf(N); }
		else if(sType == "spinOne") { sites = SpinOne(N);  }
		else if(sType == "spinTwo") { sites = SpinTwo(N);  } 
		return sites;
	}
};
#endif