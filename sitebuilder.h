#ifndef __SITEBUILDER_H_
#define __SITEBUILDER_H_
#include "itensor/all.h"
#include "itensor/mps/sites/spintwo.h"
#include "builder.h"
#include "thermalchain.h"
#include "chain.h"

/* TODO: MAKE A REAL CACHE, THIS IS SUPER JANKY.*/
SiteSet Cache;
bool readIn = false;

class SiteBuilder : public Builder
{
protected:
public:
	SiteBuilder(){}
	~SiteBuilder(){}
	SiteSet build(InputGroup* input)
	{
		if(readIn) {cout << "reading cache. " << endl; return Cache; }
		SiteSet sites;
		auto sType = input->getString("siteType","spinHalf");
		auto N = input->getInt("N",100);
		if     (sType == "spinHalf"){ sites = SpinHalf(N); }
		else if(sType == "spinOne") { sites = SpinOne(N);  }
		else if(sType == "spinTwo") { sites = SpinTwo(N);  } 
		Cache = sites;
		readIn = true;
		return sites;
		// TODO: You have no validator to make sure sites was created.
	}
};
#endif