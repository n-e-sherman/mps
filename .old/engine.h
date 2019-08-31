//The following ifndef/define/endif pattern is called a 
//scope guard, and prevents the C++ compiler (actually, preprocessor)
//from including a header file more than once.
#ifndef __ENGINE_H_
#define __ENGINE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "krylov.h"
#include "krylovfull.h"
#include <cmath>

using namespace itensor;
using namespace std;

class Engine
{
    int N;
    int c;
    std::string name_;
    MPS psi;
    MPS psi0;
    MPS A; // |A> = A|psi>
    MPO H;
    MPS correction;
    SpinHalf sites;
    Real energy;
    Krylov K;
    KrylovFull KF;
    CMatrix T;
    InputGroup input;
    Args args;
    public:
    
    //Default constructor
    Engine() : N(0) { }

    Engine(int i) : N(i) {}

    Engine(InputGroup a) : input(a) {}

    void
    read()
    {
        psi = readFromFile<MPS>("psi");
        psi0 = readFromFile<MPS>("psi");
        H = readFromFile<MPO>("H");
        energy = readFromFile<Real>("E");
        sites = readFromFile<SpinHalf>("sites");
        N = psi.length();
    }

    void
    write()
    {
        writeToFile("psi0",psi0);
        writeToFile("psi",psi);
        writeToFile("H",H);
        writeToFile("E",energy);
        writeToFile("sites",sites);
    }

    void
    readCor()
    {
        correction = readFromFile<MPS>("correction");
    }

    void
    writeCor()
    {
        writeToFile("correction",correction);
    }

    auto
    setH(std::string s)
    {
        if(s=="Heisenberg")
        {
            sites = SpinHalf(N);
            auto ampo = AutoMPO(sites);
            for(auto j : range1(N-1))
            {
                ampo += 0.5,"S+",j,"S-",j+1;
                ampo += 0.5,"S-",j,"S+",j+1;
                ampo +=     "Sz",j,"Sz",j+1;
            }
            H = toMPO(ampo);
            return H;
        }
        else
        {
            cout << "Unknown Hamiltonian." << endl;
        }
        auto ampo = AutoMPO(SpinHalf(N));
        return toMPO(ampo);
    }

    void
    setPsi0(std::string s)
    {
        if(s=="Heisenberg")
        {
            auto state = InitState(sites);
            for(auto i : range1(N))
                {
                if(i%2 == 1) state.set(i,"Up");
                else         state.set(i,"Dn");
                }
            psi0 = MPS(state);
        }
        else
        {
            cout << "Unknown Hamiltonian." << endl;
        }
    }

    std::tuple<Real,MPS> groundState(int s, Sweeps sweeps)
    {
        println(sweeps);
        for (int j = 1; j < 7; j++) cout << inds(psi0(j)) << endl;
        auto [e,p] = dmrg(H,psi0,sweeps,"Quiet");
        energy = e;
        psi = p;
        cout << "inds for ground state." << endl;
        for (int j = 1; j < 7; j++) cout << inds(psi(j)) << endl;
        cout << inner(psi0,psi) << endl;
        return std::tuple<Real,MPS>(energy,psi);
    }

    void
    print()
    {
        printfln("\nGround State Energy = %.10f",inner(psi,H,psi)/(N-1));
    }

    void
    krylov(int maxIter, Real omega, Cplx eta)
    {
        Args args;
        args.add("MaxIter",maxIter);
        args.add("UseSVD",true);
        auto Heff = LocalMPO(H);
        c = int(N/2);
        psi.position(c);
        auto newpsi = noPrime(psi(c) * op(sites,"Sz", c));
        psi.set(c, newpsi);
        psi.normalize();
        psi.position(1);

        // for (int j = 1; j < 7; j++) cout << inds(psi0(j)) << endl;
        correction = MPS(psi);
        correction.position(1);
        for(int i = 1; i < N; i++)
        {
            Heff.position(i,psi);
            auto phi = psi(i)*psi(i+1);
            auto scale = norm(phi);
            phi /= scale;
            auto res = correction(i)*correction(i+1);
            auto is2 = res.inds();
            K = Krylov(energy);
            auto nMax = K.calcT(Heff,phi,args);
            phi *= scale;
            res *= scale;
            K.calcCorrection(omega,eta,res);
            res.replaceInds(res.inds(),is2);
            psi.svdBond(i,phi,Fromleft,args);
            correction.svdBond(i,res,Fromleft,args);
        }
        return;
    }

    void 
    fullT(int maxIter)
    {
        args.add("MaxIter",maxIter);
        args.add("UseSVD",true);
        args.add("Normalize",false);
        args.add("Method","Fit");
        args.add("MaxDim",input.getInt("maxDim",200));
        c = int(N/2);
        args.add("C",c);
        psi.position(c);
        auto newpsi = noPrime(psi(c) * op(sites,"Sz", c));
        psi.set(c, newpsi);
        psi.normalize();
        psi.position(c);
        H.position(c);
        KF = KrylovFull(energy,sites);
        cout << "Built krylovfull." << endl;
        auto calcT = input.getYesNo("calcT",1);
        auto nMax = KF.calcT(H,psi,args);
        cout << "Calculated T." << endl;
    }

    void 
    fullCor(Real omega, Cplx eta)
    {
        KF.calcCorrection(omega,eta,correction,args);
    }

    void 
    krylovFull(int maxIter, Real omega, Cplx eta)
    {
        Args args;
        args.add("MaxIter",maxIter);
        args.add("UseSVD",true);
        args.add("Normalize",false);
        args.add("Method","Fit");
        args.add("MaxDim",input.getInt("maxDim",200));
        c = int(N/2);
        args.add("C",c);
        psi.position(c);
        auto newpsi = noPrime(psi(c) * op(sites,"Sz", c));
        psi.set(c, newpsi);
        psi.position(c);
        H.position(c);
        KF = KrylovFull(energy,sites);
        cout << "Built krylovfull." << endl;
        auto calcT = input.getYesNo("calcT",1);
        if(calcT)
        {
            auto nMax = KF.calcT(H,psi,args);
            KF.write();
        }
        else
        {
            KF.read();
        }
        
        KF.calcCorrection(omega,eta,correction,args);
        auto writeC = input.getYesNo("writeC",1);
        if(writeC)
        {
            writeCor();
        }
        cout << "Calculated Correction." << endl;
    }

    Real
    spectral(Real qfactor)
    {
        auto q = qfactor*M_PI*(Real(N)/Real(N+1));
        auto factor = -(1.0/M_PI)*sqrt(2.0/Real(N+1));
        Real res = 0;
        for(int i = 1; i < N; i++)
        {
            psi0.position(i);
            psi = MPS(psi0);
            auto newpsi = noPrime(psi(i) * op(sites,"Sz", i));
            psi.set(i,newpsi);
            auto val = innerC(psi,correction).imag();
            res += (val*factor*sin((i-c)*q));
        }
        return res;
    }

    void
    test(Real omega, Cplx eta)
    {
        psi = MPS(psi0);
        int c = int(N/2);
        psi.position(c);
        auto newpsi = noPrime(psi(c) * op(sites,"Sz", c));
        psi.set(c, newpsi);
        psi.position(c);
        auto z = omega + eta + energy;
        // cout << inner(psi,psi) << endl;
        // cout << innerC(correction,correction) << endl;
        H.position(c);
        correction.position(c);
        correction.replaceSiteInds(siteInds(psi));
        Print(errorMPOProd(sum(z*correction,-1*psi),H,correction));
    }

    void
    checkOrthogonal()
    {
        KF.checkOrthogonal();
    }

};
#endif //__MY_CLASS_H
