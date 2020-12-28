#ifndef __STATE_H_
#define __STATE_H_

#include "sites/sites.h"
#include <string>

using namespace itensor;
using namespace std;

class State
{
protected:

	MPS state;
	Real E0 = 0;

public:

	State() {}
	State(MPS m) : state(m) {}
	State(MPS m, Real r) : state(m), E0(r) {}
	MPS& getState() { return state; }
	void setState(MPS psi) { state = MPS(psi); }
	Real getE0() {return E0; }
	State(State const& other){ E0 = other.E0; state = other.state; }
	State dag() { auto temp = MPS(state); return State(temp.dag()); }

	/* ITensor wrapping. */
	void scale(Real s) { state *= s; }
	void scale(Cplx s) { state *= s; }
	void position(int i) { state.position(i); }
	void dagIP() { state.dag(); }  // Does this work?
	Real normalize() {auto nrm = state.normalize(); return nrm; }
	void orthogonalize() { state.orthogonalize(); }
	Real norm() {return itensor::norm(state); }
	int length() { return state.length(); }
	Real averageLinkDim() { return itensor::averageLinkDim(state); }
	int maxLinkDim() { return itensor::maxLinkDim(state); }
	void replaceSiteInds(IndexSet const& _sites) { state.replaceSiteInds(_sites); }
	void replaceLinkInds(IndexSet const& _links) { state.replaceLinkInds(_links); }
	IndexSet siteInds() { return itensor::siteInds(state); }
	IndexSet linkInds() { return itensor::linkInds(state); }
	ITensor const& operator()(int i) const { return state(i); }
	
	State sub(State& other,Args _args)
	{
		auto right = other.getState();
		auto res = sum(state,-1*right,_args);
		return State(res);
	}

	void read(istream& is)
    {
        itensor::read(is,E0);
        itensor::read(is,state);
    }
    void write(ostream& os) const
    {
        itensor::write(os,E0);
        itensor::write(os,state);
    }
};



#endif