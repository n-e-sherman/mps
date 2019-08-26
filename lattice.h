#ifndef __LATTICE_H_
#define __LATTICE_H_
#include <vector>
#include "service.h"
#include "itensor/all.h"

using namespace std;
using namespace itensor;

class Lattice : public Service
{
public:
	enum bondType
	{
		physical = 0,
		environment = 1
	};
protected:
	struct bond
	{
		bond(int x1, int x2, bondType s) {s1=x1; s2=x2; t=s;}
		int s1 = 0;
		int s2 = 0;
		bondType t = physical;
	};
	vector<bond> bonds;

public:

	Lattice() {};
	Lattice(int N) {};
	Lattice(Args a) : Service(a) {};
	void calc() {};

	vector<bond>::iterator begin(){return bonds.begin();};
	vector<bond>::iterator end() { return bonds.end(); }


};

#endif