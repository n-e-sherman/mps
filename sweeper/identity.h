#ifndef __IDENTITY_H_
#define __IDENTITY_H_
#include <vector>
#include <string>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include "sweeper/sweeper.h"


using namespace std;
using namespace itensor;

class Identity : public Sweeper
{
protected:
	int numCenter;
	Args* args;
public:

	Identity(Args* a) : Sweeper(a) {}
	Identity(){}
	~Identity(){}

	virtual void update(LocalMPO const& PH, ITensor& phi) { return; } // implemented for each type, updates phi inplace.
	virtual void update(ITensor const& PH, ITensor& phi) { return; } // implemented for each type, updates phi inplace.
	void sweep(MPO& H, MPS& psi){ return; }

};

#endif