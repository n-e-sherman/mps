#include "itensor/all.h"
#include "engine.h"
#include "krylov.h"
#include "krylovfull.h"
#include <iostream>
#include <cstdlib>

using namespace itensor;
using namespace std;

vector<Real>
linspace(Real a, Real b, int n)
{
    vector<Real> res;
    Real val = a;
    Real step = (b-a)/(n-1);
    for(int i = 0; i < n; i++)
    {
        res.push_back(a + step*i);
    }
    return res;
}

int 
main(int argc, char* argv[])
{
    auto input = InputGroup("input","input");
    // if(argc > 2)
    //     input = InputGroup(argv[1],"input");
    auto calcGS = input.getYesNo("dmrg",0);
    auto calcCor = input.getYesNo("cor",0);
    auto calcCorFull = input.getYesNo("corFull",0);
    auto test = input.getYesNo("test",0);
    auto testFull = input.getYesNo("testFull",0);
    auto checkOrthogonal = input.getYesNo("checkOrthogonal",0);
    if(calcGS)
    {
        cout << "Calculating ground state." << endl;
        auto N = input.getInt("N",100);
        auto nsweeps = input.getInt("nsweeps",5);
        auto model = input.getString("model","Heisenberg");
        auto table = InputGroup(input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table);
        Engine eng(N);
        eng.setH(model);
        eng.setPsi0(model);
        auto [energy,psi] = eng.groundState(nsweeps,sweeps);
        eng.write();
    }
    if(calcCor)
    {
        cout << "Calculating correction vector." << endl;
        Engine eng = Engine();
        eng.read();
        auto wi = input.getReal("wi",0);
        auto wf = input.getReal("wf",3);
        auto n  = input.getInt("nw",31);
        auto maxIter = input.getInt("maxIter",50);
        Cplx eta = Cplx(0,input.getReal("eta",0.1));
        auto qfactor = input.getReal("qfactor",0.5);
        auto omegas = linspace(wi,wf,n);
        vector<Real> res;
        for(auto w : omegas)
        {
            eng.krylov(maxIter,w,eta);
            cout << "krylov done." << endl;
            auto val = eng.spectral(qfactor);
            eng.test(w,eta);
            res.push_back(val);
            cout << w << "," << val << endl;
        }
        ofstream f;
        f.open("spectral");
        for(auto i : range(omegas.size()))
        {
            f << omegas[i] << "," << res[i] << endl;
        }
        f.close();
    }

    if(calcCorFull)
    {
        cout << "Calculating correction vector." << endl;
        Engine eng = Engine(input);
        eng.read();
        auto wi = input.getReal("wi",0);
        auto wf = input.getReal("wf",3);
        auto n  = input.getInt("nw",31);
        auto maxIter = input.getInt("maxIter",50);
        Cplx eta = Cplx(0,input.getReal("eta",0.1));
        auto qfactor = input.getReal("qfactor",0.5);
        eng.fullT(maxIter);
        auto omegas = linspace(wi,wf,n);
        vector<Real> res;
        for(auto w : omegas)
        {
            // eng.krylovFull(maxIter,w,eta);
            eng.fullCor(w,eta);
            cout << "krylov done." << endl;
            auto val = eng.spectral(qfactor);
            res.push_back(val);
            cout << w << "," << val << endl;
            eng.test(w,eta);
        }
        ofstream f;
        f.open("spectral");
        for(auto i : range(omegas.size()))
        {
            f << omegas[i] << "," << res[i] << endl;
        }
        f.close();
    }

    if(test)
    {
        cout << "Testing." << endl;
        auto w = input.getReal("wi",0);
        auto eta = Cplx(0,input.getReal("eta",0.1));
        Engine eng = Engine();
        eng.read();
        auto maxIter = input.getInt("maxIter",50);
        eng.krylov(maxIter,w,eta);
        eng.test(w,eta);
    }

    if(checkOrthogonal)
    {
        Engine eng = Engine(input);
        eng.read();
        auto maxIter = input.getInt("maxIter",50);
        eng.fullT(maxIter);
        eng.checkOrthogonal();
    }

    if(testFull)
    {
        cout << "Testing full." << endl;
        auto w = input.getReal("wi",0);
        auto eta = Cplx(0,input.getReal("eta",0.1));
        Engine eng = Engine(input);
        eng.read();
        auto maxIter = input.getInt("maxIter",50);
        eng.krylovFull(maxIter,w,eta);
        eng.readCor();
        eng.test(w,eta);
    }

    return 0;
}


