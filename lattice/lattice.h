#ifndef __LATTICE_H_
#define __LATTICE_H_
#include <vector>
#include <string>
#include "infrastructure/service.h"
#include "itensor/all.h"


using namespace std;
using namespace itensor;

class Lattice
{
public:
	enum type
	{
		physical = 0,
		environment = 1
	};
protected:
	struct bond
	{
		int s1 = 0;
		int s2 = 0;
		type t = physical;
		// bond(int x1, int x2, bondType s) {s1=x1; s2=x2; t=s;}
	};
	struct site
	{
		int s = 0;
		type t = physical;
	};

protected:

	/* Outputs */
	vector<bond> bonds;
	vector<site> sites;
	int N;

public:

	Lattice() {}
	Lattice(int L) : N(L) {}
	~Lattice(){}
	vector<site> getSites(){ return sites;}
	int length(){ return N; }
	vector<bond>::iterator begin(){ return bonds.begin(); }
	vector<bond>::iterator end(){ return bonds.end(); }
	int getN(){ return N; }
	static string getHash(Args* args)
	{
		string s = "";
		if(args->getBool("thermal")) s = "thermal";
		return to_string(args->getInt("N")) + "_" + s + args->getString("Lattice");
	}

	/* Saving is useless here... */
	// virtual void load(ifstream & f)
	// {
	// 	read(f,bonds);
	// 	read(f,sites);
	// 	read(f,N);
	// }
	// virtual void save(ofstream & f)
	// {
	// 	write(f,bonds);
	// 	write(f,sites);
	// 	write(f,N);
	// }



};

#endif