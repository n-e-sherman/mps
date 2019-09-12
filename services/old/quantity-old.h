#ifndef __QUANTITY_H_
#define __QUANTITY_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include <iomanip>
// #include "krylov.h"
#include "util.h"
#include <cmath>

using namespace itensor;
using namespace std;

// This class will calulcate the spectral function
// read&write will follow the naming convention
// "modelname_lattice_systemsize_eta_qfactor_w_quantity"
class Quantity
{
protected:
	Model *M;
	MPO H;
	MPS psi0; // Ground state.
	Real E0;
	int N;
	int c;
	string dataDir;
	string resDir;
	SiteSet sites; // May want to generalize this?
	IndexSet is;
    InputGroup input;

	/////////// IN SUBCLASS ////////////

public:

	Quantity() {}
	Quantity(Model* m,InputGroup i) : input(i) {M = m;}
	virtual void calculate() = 0;
	void
    set()
    {
        H = M->getH();
        psi0 = M->getPsi0();
        is = siteInds(psi0);
        N = psi0.length();
        c = int(N/2);
        sites = M->getSites();
        E0 = M->getE0();
        dataDir = M->getDataDir();
        resDir = M->getResDir();
    }

	void
	prepare(MPS &a, MPS &b)
	{
	    a.replaceSiteInds(is);
	    b.replaceSiteInds(is);
	    a.position(c);
	    b.position(c);
	}

};
#endif