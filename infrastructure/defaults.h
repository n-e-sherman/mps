#ifndef __DEFAULTS_H_
#define __DEFAULTS_H_

#include "itensor/all.h"

using namespace itensor;
using namespace std;

Args* default_global()
{
	Args* args = new Args();

/* globals */
    /* calculation type */
	args->add("Chebyshev",false);
    args->add("Correlation",false);
    args->add("KZM",false);
    args->add("Magnetization",false);
    args->add("StaticCorrelation",false);

    /* system parameters */
    args->add("N",100);
    args->add("Nx",20);
    args->add("Ny",5);
    args->add("MaxDim",500);
    args->add("time-tau",0.1);
    args->add("beta-tau",0.1);
    args->add("thermal",true); // Lattice, Measurement, State
    args->add("momentum",false); // ChebService, CorrService, Measurement, Operator

/* services */

    args->add("save",true);
    /* chebyshevService */
    args->add("nChebyshev",10);
    args->add("momentum",false);
    args->add("CenterSite",true);
    args->add("Connected",true);

    /* correlationService */
    args->add("time",1);

    /* kzmService */
    args->add("g0",5.0);
    args->add("gf",1.0);
    args->add("v",1.0);

    /* magnetizationService */

/* service objects */

    args->add("load",true);
    /* chebyshev */
    args->add("W",8);
    args->add("Wp",0.9875);
    args->add("MPOMaxDim",20);
    args->add("MaxDim",100);
    args->add("Method","DensityMatrix");

    /* correlation */
    
    args->add("momentum",false);
    args->add("time-tau",0.1); // shouldn't be here.

    /* kzm */
    args->add("J",1.0);
    args->add("g0",5.0);
    args->add("v",1.0);
    args->add("beta-sweeps",100);
    args->add("time-sweeps",100);

    /* static */
    args->add("load",true);
    args->add("Static","StaticCorrelation");
        /* Magnetization */
        args->add("BMin",0);
        args->add("BMax",1);
        args->add("BStep",0.1);
        args->add("betaMin",0);
        args->add("betaMax",1);
        args->add("betaStep",0.1);
        /* Correlations */
        args->add("qxs","");
        args->add("qys","");
        args->add("qzs","");

/* objects */

    /* lattice */
    args->add("Lattice","Chain");
        /* Chain */
        /* ThermalChain */
        /* Triangular */
        args->add("YPeriodic",true);
        args->add("Geometry","YC");

    /* sites */
    args->add("SiteSet","SpinHalf");
    args->add("ConserveQNs",true);
    args->add("ConserveSz",true);
    args->add("ConserveQNs",true);

    /* model */
    args->add("Model","Heisenberg");
        /* Heisenberg */
        /* HeisenbergField */
        args->add("B",1.0);
        /* HeisenbergLR */
        args->add("J2",0.0);
        args->add("J3",0.0);
        args->add("J4",0.0);
        /* XX */
        /* XXZ */
        args->add("Jxy",1.0);
        args->add("Delta",1.0);
        /* Dimer */
        args->add("Je",1.0);
        args->add("Jo",1.0);

    /* operator */
    // args->add("Operator","Position"); // <--- Useless b/c of "momentum" and/or "thermal" parameter
        /* Position */
        args->add("localOperator","Sz");
        /* Momentum */
        args->add("localOperator","Sz");
        args->add("momentumFunction","cos");
        args->add("qx",0.0);
        args->add("qy",0.0);
        args->add("qz",0.0);
        args->add("qFactor",1.0);

    /* evolver */

    args->add("Evolver","TDVP");
    args->add("realTime",true);
    args->add("time-tau",0.1); // Should be in the evolver, not in the parent.
    args->add("beta-tau",0.1);
        /* TDVP */
        args->add("swap",true);
        args->add("DoNormalize",false);
        args->add("Quiet",true);
        args->add("NumCenter",2);
        args->add("MaxIter",10);
        // Other sweeper parameters if desired.
            /* Basis Extension*/
            args->add("Cutoff",1E-8); // May interfere with other modules?
            args->add("Quiet",true);
            args->add("DoNormalize",false);
            args->add("NBasis",3);
            args->add("KrylovOrd",3);
            args->add("epsK",1E-12);
            args->add("Nsweep",3);
        /* Trotter */
        args->add("Normalize",false);
        args->add("MinDim",0);
        /* ExpMPO */
        args->add("MaxDim",500);
        /* Mixed */ // <--- Just concat TDVP and Trotter. Should we just remove this?

    /* state */
    // args->add("State","ThermalState"); // <--- Useless b/c of "momentum" and/or "thermal" parameter
        /* Thermal State */
        args->add("eps",1E-9);
        args->add("beta",0);
        /* Ground State */
        args->add("initial","AF");
        args->add("nSweeps",5);
        args->add("sweeps_maxdim","80,100,150,150,200");
        args->add("sweeps_mindim","20,20,10,10,10");
        args->add("sweeps_cutoff","1E-6,1E-8,1E-10,1E-12,1E-12");
        args->add("sweeps_niter","4,3,2,2,2");
        args->add("sweeps_noise","1E-7,1E-8,1E-10,0,0");

    /* measurement */
    args->add("Measurement","RealSpace");
    args->add("imaginary",true);
        /* RealSpace */
        args->add("localOperator","Sz");
        /* KSpace */
        args->add("qFactor",1.0); // <--- Want to swap to regular q at some point
        args->add("localOperator","Sz");
        /* Local */
        args->add("localOperator","Sz");

    /* repository */
    args->add("read",true);
    args->add("write",true);
    args->add("dataDir","./");
    args->add("resDir","./");

    /* sweeper */

	return args;
    
}

/****************** Services *********************/

Args* default_chebyshevService()
{
	Args* args = new Args();
	return args;
}

Args* default_correlationService()
{
	Args* args = new Args();
	return args;
}

Args* default_kzmService()
{
	Args* args = new Args();
	return args;
}

Args* default_magnetizationService()
{
	Args* args = new Args();
	return args;
}

/****************** Objects *********************/

Args* default_chebyshev()
{
	Args* args = new Args();
	return args;
}

Args* default_correlation()
{
	Args* args = new Args();
	return args;
}

Args* default_kzm()
{
	Args* args = new Args();
	return args;
}

Args* default_static()
{
	Args* args = new Args();

	return args;
}

Args* default_lattice()
{
	Args* args = new Args();
	return args;
}

Args* default_sites()
{
	Args* args = new Args();
	return args;
}

Args* default_model()
{
	Args* args = new Args();
	return args;
}

Args* default_operator()
{
	Args* args = new Args();
	return args;
}

Args* default_evolver()
{
	Args* args = new Args();
	return args;
}

Args* default_state()
{
	Args* args = new Args();
	return args;
}

Args* default_measurement()
{
	Args* args = new Args();
	return args;
}

Args* default_repository()
{
	Args* args = new Args();
	return args;
}

Args* default_sweeper()
{
	Args* args = new Args();
	return args;
}

#endif

// args["chebyshevService"] = default_chebyshevService();
// args["correlationService"] = default_correlationService();
// args["kzmService"] = default_kzmService();
// args["magnetizationService"] = default_magnetizationService();
// args["repository"] = default_repository();
// args["lattice"] = default_lattice();
// args["sites"] = default_sites();
// args["model"] = default_model();
// args["operator"] = default_operator();
// args["sweeper"] = default_sweeper();
// args["evolver"] = default_evolver();
// args["state"] = default_state();
// args["measurement"] = default_measurement();
// args["chebyshev"] = default_chebyshev();
// args["correlation"] = default_correlation();
// args["kzm"] = default_kzm();
// args["static"] = default_static();