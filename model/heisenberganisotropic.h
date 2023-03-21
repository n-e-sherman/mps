#ifndef __HEISENBERGANISOTROPIC_H_
#define __HEISENBERGANISOTROPIC_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class HeisenbergAnisotropic : public Model
{
protected:
    Real J1x;
    Real J1y;
    Real J1z;
    Real J2x;
    Real J2y;
    Real J2z;
	Real hx;
    Real hy;
    Real hz;

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
	    auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
        		if((b.n == 1)) // 1st neighbor
        		{
                    if (J1x != 0)
            			ampo += J1x,"Sx",b.s1,"Sx",b.s2;
                    if (J1y != 0)
    	                ampo += J1y,"Sy",b.s1,"Sy",b.s2;
                    if (J1z != 0)
    	                ampo += J1z,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 2)) // 2nd neighbor
        		{
                    if (J2x != 0)
            			ampo += J2x,"Sx",b.s1,"Sx",b.s2;
                    if (J2y != 0)
    	                ampo += J2y,"Sy",b.s1,"Sy",b.s2;
                    if (J2z != 0)
    	                ampo += J2z,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        }
        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    if (hx != 0)
                        ampo +=   hx,"Sx",s.s;
                    if (hy != 0)
                        ampo +=   hy,"Sy",s.s;
                    if (hz != 0)
                        ampo +=   hz,"Sz",s.s;
                }    
            }
        }

        
        if(shift != 0) calcAmpoShift(ampo, shift);
        return ampo;
	}
	
	virtual AutoMPO calcAmpoL(bool skip = false, Real shift = 0)
	{
		/* NOT IMPLEMENTED */
        Error("CalcAmpoL has not been implented for HeisenbergLR");
        auto ampo = AutoMPO(sites->getSites());
        return ampo;
	}

	vector<gate> calcGatesH()
	{
		/* NOT IMPLEMENTED */
		Error("CalcGatesH has not been implented for HeisenbergLR");
        vector<gate> gates;
        return gates;
	}

	vector<gate> calcGatesL()
	{
		/* NOT IMPLEMENTED */
		Error("CalcGatesL has not been implented for HeisenbergLR");
        vector<gate> gates;
        return gates;
	}

	virtual void setParams()
    {
        J1x = args->getReal("J1x");
        J1y = args->getReal("J1y");
        J1z = args->getReal("J1z");
        J2x = args->getReal("J2x");
        J2y = args->getReal("J2y");
        J2z = args->getReal("J2z");
        hx = args->getReal("hx");
        hy = args->getReal("hy");
        hz = args->getReal("hz");
        params["J1x"] = J1x;
        params["J1y"] = J1y;
        params["J1z"] = J1z;
        params["J2x"] = J2x;
        params["J2y"] = J2y;
        params["J2z"] = J2z;
        params["hx"] = hx;
        params["hy"] = hy;
        params["hz"] = hz;
        _enforce_no_QNs();
    }

    void _enforce_no_QNs()
    {
        auto CQN = args->getBool("ConserveQNs");
        auto CSz = args->getBool("ConserveSz");
        if((!CQN) && (!CSz)) return;
        
        auto cache = Cache::getInstance();
        auto global = (Args*)cache->load("global");
        global->add("ConserveSz", false);
        global->add("ConserveQNs", false);
        cache->save("global", global);
        args->add("ConserveSz", false);
        args->add("ConserveQNs", false);
    }

public:
    
	HeisenbergAnisotropic(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }
};
#endif
