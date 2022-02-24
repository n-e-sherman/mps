#ifndef __HEISENBERGLRFIELD_H_
#define __HEISENBERGLRFIELD_H_

#include "model/model.h"

using namespace itensor;
using namespace std;

class HeisenbergLRField : public Model
{
protected:
	Real J2;
	Real J3;
    Real J4;
    Real  B;

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0)
	{
	    auto ampo = AutoMPO(sites->getSites());
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
        		if(b.n == 1) // 1st neighbor
        		{
        			ampo += 0.5,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5,"S-",b.s1,"S+",b.s2;
	                ampo +=     "Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += 0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo +=     J2,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 3) & (J3 != 0.0)) // 3rd neighbor
        		{
        			ampo += 0.5*J3,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J3,"S-",b.s1,"S+",b.s2;
	                ampo +=     J3,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        }
        if(J4 != 0)
        {
            for(auto r : lattice->getRings())
            {
                ampo +=         J4*r.sign,"Sz",r.s1,"Sz",r.s2,"Sz",r.s3,"Sz",r.s4;
                ampo +=     0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S+",r.s3,"S-",r.s4;
                ampo +=     0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S-",r.s3,"S+",r.s4;
                ampo +=     0.5*J4*r.sign,"S+",r.s1,"S-",r.s2,"Sz",r.s3,"Sz",r.s4;
                ampo +=     0.5*J4*r.sign,"S-",r.s1,"S+",r.s2,"Sz",r.s3,"Sz",r.s4;
                ampo +=    0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S+",r.s3,"S-",r.s4;
                ampo +=    0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S-",r.s3,"S+",r.s4;
                ampo +=    0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S+",r.s3,"S-",r.s4;
                ampo +=    0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S-",r.s3,"S+",r.s4;
            }
        }

        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    ampo +=   B,"Sz",s.s;    
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
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
        		if(b.n == 1) // 1st neighbor
        		{
        			ampo += 0.5,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5,"S-",b.s1,"S+",b.s2;
	                ampo +=     "Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += 0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo +=     J2,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 3) & (J3 != 0.0)) // 3rd neighbor
        		{
        			ampo += 0.5*J3,"S+",b.s1,"S-",b.s2;
	                ampo += 0.5*J3,"S-",b.s1,"S+",b.s2;
	                ampo +=     J3,"Sz",b.s1,"Sz",b.s2;
        		}   
        	}
        	if(b.t == Lattice::environment)
        	{
        		if(b.n == 1) // 1st neighbor
        		{
        			ampo += -0.5,"S+",b.s1,"S-",b.s2;
	                ampo += -0.5,"S-",b.s1,"S+",b.s2;
	                ampo += -1.0,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 2) & (J2 != 0.0)) // 2nd neighbor
        		{
        			ampo += -0.5*J2,"S+",b.s1,"S-",b.s2;
	                ampo += -0.5*J2,"S-",b.s1,"S+",b.s2;
	                ampo += -1.0*J2,"Sz",b.s1,"Sz",b.s2;
        		}
        		if((b.n == 3) & (J3 != 0.0)) // 3rd neighbor
        		{
        			ampo += -0.5*J3,"S+",b.s1,"S-",b.s2;
	                ampo += -0.5*J3,"S-",b.s1,"S+",b.s2;
	                ampo += -1.0*J3,"Sz",b.s1,"Sz",b.s2;
        		}
        	}
        }
        if(J4 != 0)
        {

            for(auto r : lattice->getRings())
            {
                if(r.t == Lattice::physical)
                {
                    ampo +=         J4*r.sign,"Sz",r.s1,"Sz",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=     0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=     0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S-",r.s3,"S+",r.s4;
                    ampo +=     0.5*J4*r.sign,"S+",r.s1,"S-",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=     0.5*J4*r.sign,"S-",r.s1,"S+",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=    0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=    0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S-",r.s3,"S+",r.s4;
                    ampo +=    0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=    0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S-",r.s3,"S+",r.s4;    
                }
                if(r.t == Lattice::environment)
                {
                    ampo +=     -1.0*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=     -0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=     -0.5*J4*r.sign,"Sz",r.s1,"Sz",r.s2,"S-",r.s3,"S+",r.s4;
                    ampo +=     -0.5*J4*r.sign,"S+",r.s1,"S-",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=     -0.5*J4*r.sign,"S-",r.s1,"S+",r.s2,"Sz",r.s3,"Sz",r.s4;
                    ampo +=    -0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=    -0.25*J4*r.sign,"S+",r.s1,"S-",r.s2,"S-",r.s3,"S+",r.s4;
                    ampo +=    -0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S+",r.s3,"S-",r.s4;
                    ampo +=    -0.25*J4*r.sign,"S-",r.s1,"S+",r.s2,"S-",r.s3,"S+",r.s4;    
                }
                
            }
        }
        if(!skip)
        {
            for(auto s : lattice->getSites())
            {
                if(s.t == Lattice::physical)
                {
                    ampo +=   B,"Sz",s.s;    
                }
                if(s.t == Lattice::environment)
                {
                    ampo +=   -B,"Sz",s.s;    
                }
            }
        }

        if(shift != 0) calcAmpoShift(ampo, shift);
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
        J2 = args->getReal("J2");
        J3 = args->getReal("J3");
        J4 = args->getReal("J4");
        B = args->getReal("B");
        params["J2"] = J2;
        params["J3"] = J3;
        params["J4"] = J4;
        params["B"]  =  B;
    }

public:
    
	HeisenbergLRField(Args* a, Lattice* l, Sites* s) : Model(a,l,s) { setParams(); }
};
#endif
