//The following ifndef/define/endif pattern is called a 
//scope guard, and prevents the C++ compiler (actually, preprocessor)
//from including a header file more than once.
#ifndef __KRYLOV_FULL_H_
#define __KRYLOV_FULL_H_

#include <string>
#include <fstream>
#include <iostream>
#include "itensor/all.h"

using namespace itensor;
using namespace std;

class KrylovFull
{

    std::vector<MPS> V;
    CMatrix T;
    Real E0;
    int nMax;
    SpinHalf sites;
    public:

    KrylovFull(){}
    KrylovFull(Real E, SpinHalf s) : E0(E), sites(s) {}

    void
    read()
    {
        E0 = readFromFile<Real>("E0");
        nMax = readFromFile<int>("nMax");

        // T = readFromFile<CMatrix>("T");
        V = readFromFile<std::vector<MPS>>("V");
    }

    void
    write()
    {
        writeToFile("V",V);

        // writeToFile("T",T);
        writeToFile("E0",E0);
        writeToFile("nMax",nMax);
    }

    Real
    calcT(MPO const  &A, MPS const& psii, Args const& args = Args::global())
    {
        auto maxiter_ = args.getSizeT("MaxIter",2);
        auto c = args.getInt("C",50);
        auto actual_maxiter = maxiter_;

        V = std::vector<MPS>(actual_maxiter+2,MPS(psii));
        auto W = std::vector<MPS>(actual_maxiter+2,MPS(psii));
        auto WP = std::vector<MPS>(actual_maxiter+2,MPS(psii));
        T = CMatrix(actual_maxiter+2,actual_maxiter+2);
        for(auto& el : T) el = Cplx(0,0);
        V[0] = MPS(psii); // not needed?
        // PrintData(A);
        auto temp = noPrime(applyMPO(A,V[0],args));
        WP[0] = MPS(temp);
        T(0,0) = innerC(WP[0],V[0]).real();
        WP[0].noPrime();
        V[0].noPrime();
        V[0].position(c);
        WP[0].position(c);
        WP[0].replaceSiteInds(siteInds(V[0]));
        // for(auto j : range(c-5,c+5)) // WHY DOES THIS MAKE IT WORK!?!?!?!?!?
        // {
        //     cout << j << endl;
        //     cout << WP[0].ref(j) << endl;
        //     cout << V[0].ref(j) << endl;
        //     cout << "*******************" << endl;
        // }
        // WP[0].noPrime();
        // V[0].noPrime();
        W[0] = sum(WP[0],-1*T(0,0)*V[0],args);
        W[0].noPrime();
        W[0].position(c);
        W[0].replaceSiteInds(siteInds(V[0]));
        auto i = actual_maxiter;
        i=1;
        while((i < actual_maxiter) && !converged())
        {
            cout << i << endl;
            T(i-1,i) = norm(W[i-1]);
            T(i,i-1) = norm(W[i-1]);
            V[i] = (1.0/norm(W[i-1]))*W[i-1];
            WP[i] = noPrime(applyMPO(A,V[i],args));
            T(i,i) = innerC(WP[i],V[i]).real();
            V[i].noPrime();
            V[i].position(c);
            V[i].replaceSiteInds(siteInds(V[0]));
            V[i-1].noPrime();
            V[i-1].position(c);
            V[i-1].replaceSiteInds(siteInds(V[0]));
            WP[i].noPrime();
            WP[i].position(c);
            WP[i].replaceSiteInds(siteInds(V[0]));
            auto temp = sum(WP[i],-1*T(i,i)*V[i],args);
            temp.noPrime();
            temp.position(c);
            temp.replaceSiteInds(siteInds(V[0]));
            W[i] = sum(temp,-1*T(i,i-1)*V[i-1],args);
            // std::vector<MPS> sumMe{WP[i], -1*T(i,i)*V[i], -1*T(i,i-1)*V[i-1]};
            // W[i] = sum(sumMe,args);

            // auto temp = sum(WP[i],-1*T(i,i-1)*V[i-1]);
            // cout << "hi6" << endl;
            // W[i] = sum(temp,-1*T(i,i)*V[i]);

            // cout << "hi7" << endl;


            // W[i] = WP[i] - T(i,i)*V[i] - T(i,i-1)*V[i-1];
            i+=1;
        }

        nMax = int(i);
        return i;
    }


    void
    calcCorrection(Real omega, Cplx eta, MPS &res, Args const& args = Args::global())
    {
        /* Calculates 1/(z-T)|psi0> */
        // diagonal(D) &= d;


        auto z = omega + E0 + eta;
        Vector d;
        auto D = CMatrix(nMax,nMax);
        CMatrix U;
        auto Tref = subMatrix(T,0,nMax,0,nMax);
        diagHermitian(Tref,U,d);
        auto delta = CVector(nMax);
        for(int i = 0; i < nMax; i++) delta(i) = 1.0/(z-d(i));
        diagonal(D) &= delta;
        auto R = U*D*conj(transpose(U)); // 1/(z-M)
        // auto init = CVector(nMax);
        // for (auto& el : init) el = Cplx(0,0);
        // init(0) = Cplx(1,0);

        auto cor = column(R,0);



        // for(int i = 0; i < nMax; i++) DD.push_back(1.0/(-D(i) + eta + omega + E0));

        // auto cor = column(U,0); // double check this.
        // auto cor = Vector(D);
        // for(int i = 0; i < nMax; i++) cor(i) = col(i);
        // for(int i = 0; i < nMax; i++) cor(i) *= DD[i];
        // cor = mult(transpose(conj(U)),cor);
        res = MPS(V[0]*cor(0)); // Just to get correct shape, prob a better way to do this.
        for(int i = 1; i < nMax; i++) 
        {
            cout << i << endl;
            res.position(int(length(res)/2));
            res.replaceSiteInds(siteInds(V[i]));
            res.plusEq(V[i]*cor(i),args);
        }

        // for(auto &d : D.items()) DD.push_back(1.0/(-d + omega + eta + E0));
        // D = -1*D + omega + eta + E0;

        // cout << U << endl;
        // cout << D << endl;
    }

    void
    checkOrthogonal()
    {
        for(auto i : range(nMax))
        {
            for(auto j : range(nMax))
            {
                if(j >= i)
                {
                    cout << "(" << i << "," << j << ") " << innerC(V[i],V[j]) << endl;
                }
            }
        }
    }

    bool
    converged()
    {
        return false; // NEED TO MAKE A REAL CONVERGED FUNCTION
    }
};
#endif //__MY_CLASS_H
