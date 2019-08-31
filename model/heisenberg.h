#ifndef __HEISENBERG_H_
#define __HEISENBERG_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model/model.h"
#include "infrastructure/util.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Heisenberg : public Model
{
protected:
	virtual void calcH()
	{
		auto ampo = AutoMPO(sites);
        for(auto b : *lattice)
        {
        	if(b.t == Lattice::physical)
        	{
	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
	            ampo +=     "Sz",b.s1,"Sz",b.s2;
        	}
        	else
        	{
        		/* TODO: Handle Thermal */
        	}
        }
        H = toMPO(ampo);
	}
public:
	Heisenberg(){}
	Heisenberg(Lattice* l, SiteSet s, InputGroup* i) : Model(l,s,i) 
	{
		calcH();
	}
	~Heisenberg(){}

// public:
// 	Heisenberg() {}
// 	Heisenberg(InputGroup i) : Model(i) {}

// 	void
// 	build ()
// 	{
// 		makeDirs();
// 		readIn = read();
// 		if(readIn) return;
// 		sites = SpinHalf(N);
//         auto ampo = AutoMPO(sites);
//         auto lattice = squareLattice(input.getInt("N",100),input.getInt("Ny",1),{"YPeriodic=",false}); // sloppy, just a 1D chain.
//         for(auto b : lattice)
//         {
//             ampo += 0.5,"S+",b.s1,"S-",b.s2;
//             ampo += 0.5,"S-",b.s1,"S+",b.s2;
//             ampo +=     "Sz",b.s1,"Sz",b.s2;
//         }
//         H = toMPO(ampo);
      
// 	}

// 	void
// 	setPsii()
// 	{
// 		if (readIn) return;
// 		auto state = InitState(sites);
//         for(auto i : range1(N))
//             {
//             if(i%2 == 1) state.set(i,"Up");
//             else         state.set(i,"Dn");
//             }
//         psii = MPS(state);
// 	}

};
#endif
