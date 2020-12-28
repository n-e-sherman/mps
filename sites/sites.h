#ifndef __SITES_H_
#define __SITES_H_


// #include "sites/sitesbuilder.h"
#include "itensor/mps/sites/spintwo.h"
#include <string>
#include <iostream>





using namespace itensor;
using namespace std;

class Sites
{
protected:

	Args* args;
	SiteSet sites;

public:

	Sites(Args* a, SiteSet s) : args(a), sites(s) {}
	static string getHash(Args* args)
	{ 
		auto q1 = args->getBool("ConserveQNs",true);
        auto q2 = args->getBool("OddEvenUpDown",false);
        auto q3 = args->getBool("ConserveParity",false);
        string suffix = "";
	    if(q1) { suffix += "1"; } else { suffix += "0"; }
	    if(q2) { suffix += "1"; } else { suffix += "0"; }
	    if(q3) { suffix += "1"; } else { suffix += "0"; }
		return to_string(args->getInt("N")) + "_" + args->getString("SiteSet") + "_" + to_string(args->getBool("thermal")) + "_" + suffix;
	}

	SiteSet getSites(){ return sites; }

	void read(istream& is)
    {
    	auto cache = Cache::getInstance();
    	args = (Args*)cache->load("args");
    	auto sType = args->getString("SiteSet");
    	if (sType == "SpinHalf")
		{
			auto rsites = SpinHalf();
			rsites.read(is);
			sites = rsites;
		}
		else
		if (sType == "SpinOne")
		{
			auto rsites = SpinOne();
			rsites.read(is);
			sites = rsites;
		}
		else
		if (sType == "SpinTwo")
		{
			auto rsites = SpinTwo();
			rsites.read(is);
			sites = rsites;
		}
		else
		{
			sites.read(is);
		}
		cache->save(Sites::getHash(args),this);
    }

    void write(ostream& os) const
    {
    	itensor::write(os,sites);
    }
	
};

#endif
