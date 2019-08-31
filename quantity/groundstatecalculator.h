#ifndef __GROUNDSTATECALCULATOR_H_
#define __GROUNDSTATECALCULATOR_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/calculator.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"

using namespace itensor;

class GroundStateCalculator : public Calculator
{
protected:
	InputGroup* input;
	ModelBuilder* modelBuilder;
	SiteBuilder* siteBuilder;
	Model* model;
	SiteSet sites;
	MPS psi;
	Real E0;
	void calcGroundState()
	{
		/* TODO: Add the ability to read in the ground state */
		calcPsi(); // Only do if NOT read in ground state.
		auto nsweeps = input->getInt("nsweeps",5);
        auto table = InputGroup(*input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table); // how?
        Print(psi);
        Print(*(model->getH()));
        Print(applyMPO(*(model->getH()),psi));
        auto [energy,psiout] = dmrg(*(model->getH()),psi,sweeps,"Quiet");
        E0=energy;
        psi=psiout;

        // E0 = dmrg(psi,*(model->getH()),sweeps,"Quiet");
	}
	void calcPsi()
	{
		/* TODO: Check for different possible starting states, use inputs. */
		auto N = input->getInt("N",100);
		auto state = InitState(sites);
        for(auto i : range1(N))
            {
            if(i%2 == 1) state.set(i,"Up");
            else         state.set(i,"Dn");
            }
        psi = MPS(state);
	}
public:

	GroundStateCalculator(ModelBuilder* mb, SiteBuilder* sb) : Calculator() {modelBuilder = mb; siteBuilder = sb; }
	~GroundStateCalculator() {}	
	std::tuple<Real,MPS*> calculate(InputGroup* i)
	{
		input = i;
		model = modelBuilder->build(input);
		sites = siteBuilder->build(input);
		calcGroundState();
		return std::tuple<Real,MPS*>(E0,&psi);
	}
};
#endif