//The following ifndef/define/endif pattern is called a 
//scope guard, and prevents the C++ compiler (actually, preprocessor)
//from including a header file more than once.
#ifndef __ENGINE_H_
#define __ENGINE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "model.h"
#include "quantity.h"
// #include "krylov.h"
// #include "krylovfull.h"
#include <cmath>

using namespace itensor;
using namespace std;

// The "engine" to run the full calculation.
// read and write will follow the following format for naming
class Engine
{
    Model* M;
    Quantity* Q;
    InputGroup input;
    MPS correction;
    Real energy;
    CMatrix T;
    Args args;

    public:
    
    //Default constructor

    Engine(){}
    Engine(InputGroup i) : input(i) {}

    bool
    build()
    {
        // auto temp = InputGroup(input);
        M = new Model(input);
        if(!M->build()) return false;
        cout << "Engine built." << endl;
        return true;
    }

    void
    compute()
    {
        Q = new Quantity(M,input);
        Q->calculate();
    }

};
#endif //__MY_CLASS_H
