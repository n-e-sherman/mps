#ifndef __SITEBUILDER_H_
#define __SITEBUILDER_H_
#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"

class SiteBuilder
{
protected:
	SiteSet sites;
public:
	SiteBuilder(){}
	~SiteBuilder(){}
	SiteSet build(Args* args)
	{
		auto N = args->getInt("N");
		auto sType = args->getString("SiteSet");
		if     (sType == "spinHalf"){ sites = SpinHalf(N); }
		else if(sType == "spinOne") { sites = SpinOne(N);  }
		else if(sType == "spinTwo") { sites = SpinTwo(N);  } 
		return sites;
	}
};
#endif