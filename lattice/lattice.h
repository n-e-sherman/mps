#ifndef __LATTICE_H_
#define __LATTICE_H_

#include <vector>
#include <string>
#include "itensor/all.h"

using namespace std;
using namespace itensor;

class Lattice
{
public:
	enum type { physical = 0, environment = 1 };
protected:
	struct bond { int s1 = 0; int s2 = 0; type t = physical; int z = 0;	int n = 1; };
	struct ring {int s1 = 0; int s2 = 0; int s3 = 0; int s4 = 0; type t = physical; int z = 0; int n = 1; int sign = 1; };
	struct site { int s = 0; int p = 0; int a = 0; type t = physical; };

	vector<ring> rings;
	vector<bond> bonds;
	vector<site> sites;
	Args* args;

public:

	Lattice() {}
	Lattice(Args* a) : args(a) {}
	~Lattice(){}
	vector<site> getSites(){ return sites;}
	vector<ring> getRings(){ return rings;}
	int length(){ return args->getInt("N"); }
	vector<bond>::iterator begin(){ return bonds.begin(); }
	vector<bond>::iterator end(){ return bonds.end(); }

};

#endif