#ifndef __XXZJ1J2_H_
#define __XXZJ1J2_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class XXZJ1J2 : public Model
{
protected:
    Real J1;
    Real J2;
    Real Delta;
    Real hz;
    Real hx;

    virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
    {
        auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
            if(b.t == Lattice::physical)
            {
                if((b.n == 1) & (J1 != 0.0)) // 1st neighbor
                {
                    ampo += 0.5*J1,"S+",b.s1,"S-",b.s2;
                    ampo += 0.5*J1,"S-",b.s1,"S+",b.s2;
                    if (Delta != 0)
                        ampo += Delta*J1,"Sz",b.s1,"Sz",b.s2;
                    
                    
                }
                if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
                {
                    ampo += 0.5*J2,"S+",b.s1,"S-",b.s2;
                    ampo += 0.5*J2,"S-",b.s1,"S+",b.s2;
                    if (Delta != 0)
                        ampo += Delta*J2,"Sz",b.s1,"Sz",b.s2;
                }
            }
        }
        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    if(hz != 0)
                        ampo += hz,"Sz",s.s;
                    if(hx != 0)
                        ampo += hx,"Sx",s.s;
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
        J1 = args->getReal("J1");
        J2 = args->getReal("J2");
        Delta = args->getReal("Delta");
        hz = args->getReal("hz");
        hx = args->getReal("hx");
        params["J1"] = J1;
        params["J2"] = J2;
        params["Delta"] = Delta;
        params["hz"] = hz;
        params["hx"] = hx;
        if (hx != 0) _enforce_no_QNs();
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
    
    XXZJ1J2(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }
};
#endif
