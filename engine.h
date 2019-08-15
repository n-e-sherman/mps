//The following ifndef/define/endif pattern is called a 
//scope guard, and prevents the C++ compiler (actually, preprocessor)
//from including a header file more than once.
#ifndef __ENGINE_H_
#define __ENGINE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "Heisenberg.h"
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

    public:
    
    //Default constructor

    Engine(){}
    Engine(InputGroup i) : input(i) {}

    bool
    build()
    {
        // auto temp = InputGroup(input);
        getLattice();
        getModel();
        M->build();
        M->setPsii();
        M->setPsi0();
        return true;
    }

    void
    compute()
    {
        getQuantity();
        Q->calculate();
    }

    void
    getLattice()
    {
    	auto i = 0;
    }

    void
    getModel()
    {
        auto modelName = input.getString("Model","Heisenberg");
        if(modelName == "Heisenberg")
        {
            M = new Heisenberg(input);
        }
    }

    void
    getQuantity()
    {
        Q = new Quantity(M,input);
    }

};
#endif //__MY_CLASS_H
