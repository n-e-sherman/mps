#ifndef __SWEEPER_H_
#define __SWEEPER_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include "itensor/util/print_macro.h"


using namespace std;
using namespace itensor;

class Sweeper
{
protected:
	int numCenter;
	int b;
	int nsweeps;
	int N;
	Args* args;
	int nTrim;
	Real maxE = 0;
	vector<Real> details;
public:

	Sweeper(Args* a) 
	{
		args = a;
		details = vector<Real>({0,0,0,0,0});
	}
	Sweeper(){}
	~Sweeper(){}

	// template<class BigMatrixT>
	virtual vector<Real> get_details(){ return details; }
	virtual vector<string> get_labels(){ return vector<string>({"sw","nTrim","maxChi","dif","maxE"}); }
	virtual void update(LocalMPO const& PH, ITensor& phi) = 0; // implemented for each type, updates phi inplace.
	virtual void sweep(MPO& H, MPS& psi) // adjusts psi inplace.
	{
		numCenter = args->getInt("NumCenter");
		nsweeps = args->getInt("sweeperCount");
		auto dif_tol = args->getReal("difThreshold");
		N = psi.length();
		psi.position(1);
		LocalMPO PH(H,*args);
		ITensor phi;
		auto detail = vector<Real>();
		for(int sw = 1; sw <= nsweeps; ++sw)
		{
			maxE = 0;
			nTrim = 0;
			auto initial = MPS(psi);
			b = 1;
			for(int ha = 1; ha <= 2; sweepnext(b,ha,N,{"NumCenter=",numCenter}))
			{
				PH.position(b,psi);
				if(numCenter == 1) 
				{
					phi = psi(b);
					update(PH,phi);
					psi.ref(b) = phi;
				}
				else
				if(numCenter == 2) 
				{
					phi = psi(b)*psi(b+1);
					update(PH,phi);
					psi.svdBond(b,phi,(ha==1 ? Fromleft : Fromright),PH,*args);
				}
			}
			auto nrmi = inner(initial,initial);
			auto nrmf = inner(psi,psi);
			auto brkt = inner(initial,psi);
			auto dif  = (nrmi + nrmf - 2.0*brkt)/nrmi;
			auto maxdim = maxLinkDim(psi);
			auto avgdim = averageLinkDim(psi);
			details = vector<Real>({Real(sw),Real(nTrim),Real(maxdim),Real(dif),Real(maxE)});
			cout << "(sw,nTrim,maxdim,avgdim,nrmi,nrmf,dif,maxE) = (" << sw << "," << nTrim << "," << maxdim << "," << avgdim << "," << nrmi << "," << nrmf << "," << dif << "," << maxE << ")" << endl;
			if((nTrim == 0) || (std::abs(dif) < dif_tol)) break;
		}
	}
};

#endif