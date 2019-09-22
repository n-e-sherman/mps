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
		cout << "building sites" << endl;
		auto N = args->getInt("N");
		if(args->getBool("thermal")) N = 2*N;
		auto sType = args->getString("SiteSet");
		if     (sType == "SpinHalf"){ sites = SpinHalf(N); }
		else if(sType == "SpinOne") { sites = SpinOne(N);  }
		else if(sType == "SpinTwo") { sites = SpinTwo(N);  } 
		return sites;
	}
};
#endif