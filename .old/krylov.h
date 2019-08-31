//The following ifndef/define/endif pattern is called a 
//scope guard, and prevents the C++ compiler (actually, preprocessor)
//from including a header file more than once.
#ifndef __KRYLOV_H_
#define __KRYLOV_H_

#include <string>
#include <iostream>
#include "itensor/all.h"

using namespace itensor;
using namespace std;

class Krylov
{

    std::vector<ITensor> V;
    CMatrix T;
    ITensor psi0;
    Real E0;
    int nMax;
    public:

    Krylov(){}
    Krylov(Real E) : E0(E) {}

    template <class BigMatrixT>
    Real
    calcT(BigMatrixT const  &A, ITensor const& psii, Args const& args = Args::global())
    {
        psi0 = ITensor(psii);
        auto maxiter_ = args.getSizeT("MaxIter",2);
        auto maxsize = A.size();
        auto actual_maxiter = std::min(maxiter_,maxsize-1);

        V = std::vector<ITensor>(actual_maxiter+2);
        auto W = std::vector<ITensor>(actual_maxiter+2);
        auto WP = std::vector<ITensor>(actual_maxiter+2);
        T = CMatrix(actual_maxiter+2,actual_maxiter+2);
        for(auto& el : T) el = Cplx(0,0);
        V[0] = psii;
        A.product(V[0],WP[0]);
        T(0,0) = eltC((dag(WP[0])*V[0])).real();
        W[0] = WP[0] - T(0,0)*V[0];

        auto i = actual_maxiter;
        i=1;
        while((i < actual_maxiter) && !converged())
        {
            T(i-1,i) = norm(W[i-1]);
            T(i,i-1) = norm(W[i-1]);
            V[i] = (1.0/norm(W[i-1]))*W[i-1];
            A.product(V[i],WP[i]);
            T(i,i) = eltC((dag(WP[i])*V[i])).real();
            W[i] = WP[i] - T(i,i)*V[i] - T(i,i-1)*V[i-1];
            i+=1;
        }

        nMax = int(i);
        return i;
    }

    void
    calcCorrection(Real omega, Cplx eta, ITensor &res)
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
        res = V[0]*cor(0); // Just to get correct shape, prob a better way to do this.
        for(int i = 1; i < nMax; i++) res += V[i]*cor(i);

        // for(auto &d : D.items()) DD.push_back(1.0/(-d + omega + eta + E0));
        // D = -1*D + omega + eta + E0;

        // cout << U << endl;
        // cout << D << endl;
    }

    bool
    converged()
    {
        return false; // NEED TO MAKE A REAL CONVERGED FUNCTION
    }
};
#endif //__MY_CLASS_H
