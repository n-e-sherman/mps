#include "itensor/all.h"
#include "factory.h"
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
    Factory factory(&input);
    factory.run();
    return 0;
}


