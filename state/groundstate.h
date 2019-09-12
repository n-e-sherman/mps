#ifndef __GROUNDSTATE_H_
#define __GROUNDSTATE_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "repository/repositorybuilder.h"
#include "repository/repository.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/state.h"
#include <string>

using namespace itensor;
using namespace std;

class GroundState : public State
{
protected:

	void calcGroundState()
	{
		auto nsweeps = args->getInt("nsweeps",5);
        auto table = InputGroup(*input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table); // how?
        auto sweeps = Cache::getIstance()->get("sweeps");
        auto [energy,psiout] = dmrg(*(model->getH()),state,sweeps,"Quiet");
        E0=energy;
        state=psiout;
	}
	void calcPsii() // May want to make this it's own functionality? you are assuming AF GS.
	{
		/* TODO: Check for different possible starting states, use inputs. Might want to handle in statecalculator. */
		auto N = input->getInt("N",100);
		auto _state = InitState(sites);
        for(auto i : range1(N))
            {
            if(i%2 == 1) _state.set(i,"Up");
            else         _state.set(i,"Dn");
            }
        state = MPS(_state);
	}

public:

	GroundState(Args* a,Model* m, MPS& in) : State(a,m)
	{
		state = in;
		calcGroundState();
	}
	~GroundState() {}	
	// virtual stateInfo getState(){return stateInfo{&state, E0}; }
	static string getHash(Model* m)
	{
		return "GroundState_" + State::getHash(m); // Maybe add some specifications for how you get GS?
	}
	virtual string getHash() {return GroundState::getHash(model); }
};
#endif