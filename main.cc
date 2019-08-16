#include "itensor/all.h"
#include "engine.h"
#include "model.h"
#include "quantity.h"
#include <iostream>
#include <cstdlib>

using namespace itensor;
using namespace std;

int 
main(int argc, char* argv[])
{
    auto name = "input";
    if(argc >= 2)
        name = argv[1];
    auto input = InputGroup(name,"input");
    Engine eng(input);
    eng.build();
    eng.compute();
    return 0;
}


