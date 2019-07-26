#include "itensor/all.h"
#include "engine.h"
#include "model.h"
#include "quantity.h"
// #include "krylov.h"
// #include "krylovfull.h"
#include <iostream>
#include <cstdlib>

using namespace itensor;
using namespace std;

// vector<Real>
// linspace(Real a, Real b, int n)
// {
//     vector<Real> res;
//     Real val = a;
//     Real step = (b-a)/(n-1);
//     for(int i = 0; i < n; i++)
//     {
//         res.push_back(a + step*i);
//     }
//     return res;
// }

int 
main(int argc, char* argv[])
{
    auto name = "input";
    if(argc > 2)
        name = argv[1];
    auto input = InputGroup(name,"input");
    // if(argc > 2)
    //     input = InputGroup(argv[1],"input");
    Engine eng(input);
    if(!eng.build()) return EXIT_FAILURE;
    eng.compute();
    return 0;
}


