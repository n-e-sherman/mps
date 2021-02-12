#ifndef __TDVP_H_
#define __TDVP_H_

#include "evolver/evolver.h"
#include "addons/itensor-tdvp.h"
#include "addons/itensor-basisextension.h"

using namespace itensor;
using namespace std;

class TDVP : public Evolver
{
protected:

	vector<Model::gate> mgates;
	vector<BondGate> gates;
	Sites* sites;
	Model* model;

	std::vector<Real> epsilonK;
	Sweeps sweeps;
	Cplx dt;

public:

	TDVP(){}
	TDVP(Args* a, Model* m) : Evolver(a,m)
	{
		sites = model->getSites();
	}

	void evolve(State& s)
	{
		if(false) /* Make a check */
        	addBasis(s.getState(),model->getO(),epsilonK,*args);
		tdvp(s.getState(),model->getO(),dt,sweeps,*args);
	}

	void setup(BondGate::Type type, Real tau, string op = "H")
	{
		dt = Cplx(tau);
		if(type == BondGate::tReal)
		{
			dt *= Complex_i;
		}
		if(op == "H"){ model->calcH(); }
		else         { model->calcL(); }

		auto epsK = args->getReal("epsK");
        epsilonK = {epsK, epsK};

		/* Make these args parameters? */
		sweeps = Sweeps(1);
	    sweeps.maxdim() = 2000;
	    sweeps.niter() = 10;
	    //sweeps.cutoff() = 1E-12;
	}

	virtual void read(istream& is)
	{
		sites->read(is);
	}

	virtual void write(ostream& os) const
	{
		sites->write(os);
	}


};
#endif


// for(int n = 1; n <= nsw; ++n)
//         {
//         if(n < 3)
//             {
//             // Global subspace expansion
//             std::vector<Real> epsilonK = {1E-12, 1E-12};
//             addBasis(psi1,H,epsilonK,{"Cutoff",1E-8,
//                                       "Method","DensityMatrix",
//                                       "KrylovOrd",3,
//                                       "DoNormalize",true,
//                                       "Quiet",true});
//             }
        
//         // TDVP sweep
//         energy = tdvp(psi1,H,-t,sweeps,{"DoNormalize",true,
//                                         "Quiet",true,
//                                         "NumCenter",1});
//         }