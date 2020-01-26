// #ifndef __XXLOUIVILLE_H_
// #define __XXLOUIVILLE_H_

// #include <string>
// #include <iostream>
// #include "itensor/all.h"
// #include "model/model.h"
// #include "infrastructure/util.h"
// #include <cmath>

// using namespace itensor;
// using namespace std;

// class XXLouiville : public Model
// {
// protected:
// 	virtual void calcH()
// 	{
//                 for(auto b : *lattice)
//                 {
//                 	if(b.t == Lattice::physical)
//                 	{
//         	            ampo += 0.5,"S+",b.s1,"S-",b.s2;
//         	            ampo += 0.5,"S-",b.s1,"S+",b.s2;
//                 	}
//                 	if(b.t == Lattice::environment)
//                 	{
//         	            ampo += -0.5,"S+",b.s1,"S-",b.s2;
//         	            ampo += -0.5,"S-",b.s1,"S+",b.s2;
//                 	}
//                 }
// 	}
// public:
// 	XXLouiville(){}
// 	XXLouiville(Args* a) : Model(a) {}
// 	XXLouiville(Args* a, Lattice* l, SiteSet s) : Model(a,l,s) 
// 	{
//         	calcH();
//                 H = toMPO(ampo);
// 	}
// 	~XXLouiville(){}
// };
// #endif
