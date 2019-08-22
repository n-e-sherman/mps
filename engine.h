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
#include "lattice.h"
#include "structurefactor.h"
#include "spectralweights.h"
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
    Lattice lattice;

    public:
    
    //Default constructor

    Engine(){}
    Engine(InputGroup i) : input(i) {}

    void
    build()
    {
        // auto temp = InputGroup(input);
        getLattice();
        getModel();
        M->build();
        M->setPsii();
        M->setPsi0();
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
        auto modelName = input.getString("model","Heisenberg");
        if(modelName == "Heisenberg")
        {
            M = new Heisenberg(input);
        }
    }

    void
    getQuantity()
    {
    	auto quantityName = input.getString("quantity","weights");
    	if(quantityName == "weights")
	        Q = new SpectralWeights(M,input);
	    else
	    	Q = new StructureFactor(M,input);
    }

};
#endif //__MY_CLASS_H
