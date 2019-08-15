#ifndef __HEISENBERG_H_
#define __HEISENBERG_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model.h"
#include "util.h"
#include <cmath>

using namespace itensor;
using namespace std;

// This class will initialize the Hamiltonian and get site
// structure for subsequent calculations. Will also find the
// ground state of the Hamiltonian.
// read&write will follow the naming convention
// "modelname_lattice_systemsize_quantity"
class Heisenberg : public Model
{
public:
	Heisenberg() {}
	Heisenberg(InputGroup i) : Model(i) {}

	void
	build ()
	{
		makeDirs();
		readIn = read();
		if(readIn) return;
		sites = SpinHalf(N);
        auto ampo = AutoMPO(sites);
        auto lattice = squareLattice(input.getInt("N",100),input.getInt("Ny",1),{"YPeriodic=",false}); // sloppy, just a 1D chain.
        for(auto b : lattice)
        {
            ampo += 0.5,"S+",b.s1,"S-",b.s2;
            ampo += 0.5,"S-",b.s1,"S+",b.s2;
            ampo +=     "Sz",b.s1,"Sz",b.s2;
        }
        H = toMPO(ampo);
      
	}

	void
	setPsii()
	{
		if (readIn) return;
		auto state = InitState(sites);
        for(auto i : range1(N))
            {
            if(i%2 == 1) state.set(i,"Up");
            else         state.set(i,"Dn");
            }
        psii = MPS(state);
	}

};
#endif
