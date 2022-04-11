#ifndef __GROUNDSTATE_H_
#define __GROUNDSTATE_H_

#include "state/state.h"

using namespace itensor;
using namespace std;

class GroundState : public State
{

protected:

	Args* args;
	Sites* sites;
	Model* model;
	
public:

	GroundState(Args* a) : args(a) {}
	GroundState(Args* a, Model* m) : args(a), model(m)
	{
		sites = model->getSites();
		model->calcH();
		buildInitialState();
		calcGroundState();
		cout << "ground state bond-dim = " << itensor::maxLinkDim(state) << endl;
	}

private:

	void calcGroundState()
	{
        auto sweeps = getSweeps(); // how?
        auto [energy,psiout] = dmrg(model->getO(),state,sweeps,"Silent");
        E0=energy;
        state=psiout;
	}

	void buildInitialState()
	{
		auto init = args->getString("initial");
		auto N = args->getInt("N");
		auto _state = InitState(sites->getSites());
		if (init == "AF")
		{
	        for(auto i : range1(N))
            {
	            if(i%2 == 1) _state.set(i,"Up");
	            else         _state.set(i,"Dn");
            }
	        state = MPS(_state);
    	}
    	else
    	if (init == "UF")
    	{
	        for(auto i : range1(N)) _state.set(i,"Up");
	        state = MPS(_state);	
    	}
    	else
    	if (init == "DF")
    	{
	        for(auto i : range1(N)) _state.set(i,"Dn");
	        state = MPS(_state);	
    	}
    	else
    	if (init == "Striped")
    	{
	        for(auto i : range1(int(N/2)))
	        {
	        	if(i%2 == 1)
	        	{
	        		_state.set(2*i-1,"Up");
	        		_state.set(2*i,"Up");
	        	} 
	            else
	            {
                	_state.set(2*i-1,"Dn");
	        		_state.set(2*i,"Dn");
	            }
	        } 
	        state = MPS(_state);	
    	}
    	else
    	if (init == "Random")
    	{
			auto _sites = sites->getSites();
	        state = MPS(_sites);	
	    	for(auto i : range1(N))
	    	{
	    		double rndNum = (double) 2*M_PI*random()/RAND_MAX;
		         auto si = _sites(i);
		         auto wf = ITensor(si);
		         wf.set(si(1), cos(rndNum));
		         wf.set(si(2), sin(rndNum));
		         state.setA(i,wf);
	    	}
    	}
    	else state = MPS(_state);
    	/* Include other initial states with an else if.*/
	}

	Sweeps getSweeps()
	{
		Sweeps sweeps(args->getInt("nSweeps"));
		auto maxdim = stringToVectorI(args->getString("sweeps_maxdim"));
		for(auto i : range(maxdim.size())) sweeps.setmaxdim(i+1,maxdim[i]);
		auto mindim = stringToVector(args->getString("sweeps_mindim"));
		for(auto i : range(mindim.size())) sweeps.setmindim(i+1,mindim[i]);
		auto cutoff = stringToVector(args->getString("sweeps_cutoff"));
		for(auto i : range(cutoff.size())) sweeps.setcutoff(i+1,cutoff[i]);
		auto niter = stringToVector(args->getString("sweeps_niter"));
		for(auto i : range(niter.size())) sweeps.setniter(i+1,niter[i]);
		auto noise = stringToVector(args->getString("sweeps_noise"));
		for(auto i : range(noise.size())) sweeps.setnoise(i+1,noise[i]);
		return sweeps;
	}
};
#endif