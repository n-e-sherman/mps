// #ifndef __MODEL_H_
// #define __MODEL_H_

// #include <string>
// #include <iostream>
// #include "itensor/all.h"
// // #include "heisenbergchain.h"
// #include "util.h"
// #include <cmath>

// using namespace itensor;
// using namespace std;

// class ModelFactory : public Service
// {
// protected:
// 	Arg args;
// 	MPO* H;
// 	Lattice* L;
// 	SiteSet sites;
// 	int N;
	
// 	calcSites()
// 	{
// 		auto sType = args.getString("siteType","spinHalf");
// 		if     (sType == "spinHalf"){ sites = SpinHalf(N); }
// 		else if(sType == "spinOne") { sites = SpinOne(N);  }
// 		else if(sType == "spinTwo") { sites = SpinTwo(N);  } 
// 	}

// public:
// 	Model(Arg a, Lattice* l, SiteSet s) : Service(a), 
// 	{ 
// 		L = l; 
// 		sites = s;
// 		N = a.getInt("N",100);
// 		calcSites();

// 	}
// 	virtual void calc() = 0;
// 	MPO const * const getH() const {return H};

// };
// #endif
