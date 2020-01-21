#ifndef __DIMER_H_
#define __DIMER_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Dimer : public Model
{
	Real Je;
	Real Jo;
protected:
	virtual void calcH()
	{
        for(auto b : *lattice)
        {
        	// TODO: May want to modify if 2D ever happens, you assume standard ordering. May need to add something to lattice struct.
        	if(b.t == Lattice::physical)
        	{
        		if((b.s1 % 2) == 0) // Even
        		{
		            ampo += 0.5*Je,"S+",b.s1,"S-",b.s2;
		            ampo += 0.5*Je,"S-",b.s1,"S+",b.s2;
		            ampo +=     Je,"Sz",b.s1,"Sz",b.s2;
	        	}
        		else // Odd
        		{
		            ampo += 0.5*Jo,"S+",b.s1,"S-",b.s2;
		            ampo += 0.5*Jo,"S-",b.s1,"S+",b.s2;
		            ampo +=     Jo,"Sz",b.s1,"Sz",b.s2;
	        	}
        	}
        }
	}

	void calcThermalGates()
	{
		auto even = vector<ITensor>;
		auto odd = vector<ITensor>;
		for(auto b : *lattice)
		{
        	if(b.t == Lattice::physical)
        	{

        		if((b.s1 % 2) == 0) // Even
        		{
        			auto hterm = Je*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s2);
        			hterm += 0.5*Je*op(sites,"S+",b.s1)*op(sites,"S+",b.s2);
        			hterm += 0.5*Je*op(sites,"S-",b.s1)*op(sites,"S-",b.s2);
        			even.push_back(hterm);
	        	}
        		else // Odd
        		{
        			auto hterm = Jo*op(sites,"Sz",b.s1)*op(sites,"Sz",b.s2);
        			hterm += 0.5*Jo*op(sites,"S+",b.s1)*op(sites,"S+",b.s2);
        			hterm += 0.5*Jo*op(sites,"S-",b.s1)*op(sites,"S-",b.s2);
        			odd.push_back(hterm);
	        	}
        	}
		}
		
	}

public:
	Dimer(){}
	Dimer(Args* a) : Model(a) {}
	Dimer(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
	{
		Je = args->getReal("Je");
		Jo = args->getReal("Je");
		calcH();
        H = toMPO(ampo);
        calcThermalGates();
	}
	~Dimer(){}
};
#endif
