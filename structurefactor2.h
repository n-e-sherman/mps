#ifndef __SPECTRALWEIGHTS_H_
#define __SPECTRALWEIGHTS_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include <iomanip>
#include "quantity.h"
// #include "krylov.h"
#include "util.h"
#include <cmath>

using namespace itensor;
using namespace std;

// This class will calulcate the spectral function
// read&write will follow the naming convention
// "modelname_lattice_systemsize_eta_qfactor_w_quantity"
class SpectralWeights : public Quantity
{
protected:

    CMatrix T;
    CMatrix TR;
    vector<MPS> V;
    Args args;
    MPO H2;
    MPS psii; // psii = S^z_c|psi0>
    int nMax;
    Real psiiNorm;
    Real spectralNorm;
    vector<string> setas;
    vector<Cplx> etas;
    vector<Real> omegas;
    vector<vector<Real>> residues;
    vector<vector<Real>> weights;
    vector<vector<Real>> frequencies;
    vector<vector<Real>> frequenciesBare;
    CMatrix HP; 
    CMatrix HP2;
    CMatrix W; 
    CMatrix K; 
    CMatrix ST;
    CMatrix S;
    CVector Norm;

public:

    SpectralWeights() {}
    SpectralWeights(Model* m,InputGroup i) : Quantity(m,i) {}


    void
    calculate()
    {
        set();
        makeDirs();
        setPsii();
        calcResult();
    }

    void
    calcResult()
    {
        auto maxIter = input.getInt("nLanczos",200);
        psii.position(c);
        H2 = MPO(H);
        H.position(c);
        H2.position(c);
        // residues    = std::vector<std::vector<Real>>(maxIter+2,std::vector<Real>());
        // weights     = std::vector<std::vector<Real>>(maxIter+2,std::vector<Real>());
        // frequencies = std::vector<std::vector<Real>>(maxIter+2,std::vector<Real>());
        V = std::vector<MPS>(maxIter+2,MPS(psii));
        auto U = std::vector<MPS>(maxIter+2,MPS(psii));
        auto UP = std::vector<MPS>(maxIter+2,MPS(psii));

        T = CMatrix(maxIter+2,maxIter+2);
        for(auto& el : T) el = Cplx(0,0);
        TR = CMatrix(maxIter+2,maxIter+2);
        for(auto& el : TR) el = Cplx(0,0);
        HP = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : HP) el = Cplx(NAN,NAN);
        HP2 = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : HP2) el = Cplx(NAN,NAN);
        W  = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : W) el = Cplx(NAN,NAN);
        K  = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : K) el = Cplx(NAN,NAN);
        ST = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : ST) el = Cplx(NAN,NAN);
        S  = CMatrix(maxIter+2,maxIter+2);
        for (auto &el : S) el = Cplx(NAN,NAN);
        Norm  = CVector(maxIter);
        for (auto &el : Norm) el = Cplx(NAN,NAN);

        V[0] = MPS(psii);
        UP[0] = applyMPO(H,V[0],args);
        T(0,0) = innerC(UP[0],V[0]).real();
        prepare(V[0],UP[0]);
        U[0] = sum(UP[0],-1*T(0,0)*V[0],args);
        calcWeights(0);
        int i = 1;
        while((i < maxIter) && !converged())
        {
            cout << i << endl;
            T(i-1,i) = norm(U[i-1]);
            T(i,i-1) = norm(U[i-1]);
            V[i] = (1.0/norm(U[i-1]))*U[i-1];
            UP[i] = applyMPO(H,V[i],args);
            T(i,i) = innerC(UP[i],V[i]).real();
            prepare(UP[i],V[i]);
            auto temp = sum(UP[i],-1*T(i,i)*V[i],args);
            prepare(temp,V[i-1]);
            U[i] = sum(temp,-1*T(i,i-1)*V[i-1],args);
            calcWeights(i);
            i+=1;
        }
        nMax=i;
        write();

    }

    void
    calcWeights(int n) // SEEMS TO BE SOME WEIRD OFF BY 1 SHIT HERE
    { // Find TR, diagonalize, calculate all Omega_n, omega_n
        for(auto i : range(n+1)) // fill in last row and column of W,HP,HP2
        {
            W(i,n)   = innerC(V[i],V[n]);
            W(n,i)   = innerC(V[n],V[i]);
            HP(i,n)  = innerC(V[i],H,V[n]);
            HP(n,i)  = innerC(V[n],H,V[i]);
            HP2(i,n) = innerC(prime(V[i]),prime(H),H,V[n]);
            HP2(n,i) = innerC(prime(V[n]),prime(H),H,V[i]);
        }
        Norm(0) = sqrt(W(0,0));
        S(0,0) = 1.0/Norm(0);
        for(auto i : range(n+1)) {cout << "Getting S : " << i << endl; getS(i,n);}
        for(auto &el : TR) el = Cplx(0,0);

        for(auto k : range(n+1))
        for(auto m : range(n+1))
        for(auto i: range(k+1))
        for(auto j: range(m+1))
            TR(k,m) += conj(S(i,k))*S(j,m)*HP(i,j);
        
        Vector d;
        CMatrix U;
        auto Tref = subMatrix(TR,0,n+1,0,n+1);
        diagHermitian(Tref,U,d);
        Vector dBare;
        CMatrix UBare;
        auto TrefBare = subMatrix(T,0,n+1,0,n+1);
        diagHermitian(TrefBare,UBare,dBare);
        auto temp = vector<Real>();
        for(auto el : row(U,0)) temp.push_back(abs(el)*abs(el)); // Maybe column?
        weights.push_back(temp);
        temp = vector<Real>();
        for(auto el : d) temp.push_back(el);
        frequencies.push_back(temp);
        temp = vector<Real>();
        for(auto el : dBare) temp.push_back(el);
        frequenciesBare.push_back(temp);
        temp = vector<Real>();
        for(auto k : range(n+1))
        {
            auto tempH = Cplx(0,0);
            auto tempH2 = Cplx(0,0);
            for(auto i : range(n+1))
            for(auto j : range(n+1))
            for(auto p : range(i+1))
            for(auto q : range(j+1))
            {
                tempH +=  U(i,k)*U(j,k)*S(p,i)*S(q,j)*HP(p,q);
                tempH2 += U(i,k)*U(j,k)*S(p,i)*S(q,j)*HP2(p,q);
            }
            temp.push_back( abs(tempH2 + d[k]*d[k] - (2*d[k]*tempH)) );
        }
        residues.push_back(temp);

        

    }
    Cplx
    getS(int i, int j)
    {   
        if(!isnan(S(i,j).real())) return S(i,j);

        Cplx val = Cplx(0,0);
        val = getST(i,j)/getN(j); // WAS THIS A BUG!?!?!?
        S(i,j) = val;
        return val;
    }

    Cplx
    getST(int i, int j)
    {
        if(!isnan(ST(i,j).real())) return ST(i,j);

        if(i == j)
        {
            ST(i,j) = Cplx(1,0);
            return Cplx(1,0); 
        }

        Cplx val = Cplx(0,0);
        for(auto k : range(i,j)) val -= getK(k,j)*getS(i,k);
        ST(i,j) = val;
        return val;
    }

    Cplx
    getN(int j)
    {
        if(!isnan(Norm(j).real())) return Norm(j);

        Cplx val = Cplx(0,0);
        for(auto k : range(j+1))
        for(auto l : range(j+1))
            val += getST(k,j)*getST(l,j)*W(k,l);
        Norm(j) = sqrt(val);
        return Norm(j);
    }

    Cplx
    getK(int i, int j)
    {
        if(!isnan(K(i,j).real())) return K(i,j);

        Cplx val = Cplx(0,0);
        for(auto k : range(i+1)) val += getS(k,i)*W(j,k);
        K(i,j) = val;
        return val;
    }

    void
    set()
    {
        Quantity::set();
        args.add("MaxIter",input.getInt("nLanczos",50));
        args.add("UseSVD",true);
        args.add("Normalize",false);
        args.add("Method","Fit");
        args.add("MaxDim",input.getInt("maxDim",200));
        args.add("C",c);
    }

    void
    setPsii()
    {
        auto qfactor = input.getReal("qfactor",1.0);
        auto q = qfactor*M_PI*(Real(N)/Real(N+1));
        auto ampo = AutoMPO(sites);
        for(auto i : range1(N)) // THIS WILL ONLY WORK IN 1D RIGHT NOW
        {
            ampo += sin(i*q),"Sz",i;
        }
        auto Sk = toMPO(ampo);
        Sk.position(c);
        psi0.position(c);
        psii = applyMPO(Sk,psi0);
        prepare(psii,psi0);
        spectralNorm = sqrt(2.0/(N+1));
        psiiNorm = psii.normalize();
    }

    void
    write()
    {
        writeToFile(dataDir+"T",T);
        writeToFile(dataDir+"T_R",T);
        writeToFile(dataDir+"nMax",nMax);
        writeToFile(dataDir+"psiiNorm",psiiNorm);
        ofstream f;
        for(int i = 0; i < nMax; i++)
        {
            for(auto x : frequencies[i]) cout << x << endl;
            for(auto x : weights[i]) cout << x << endl;
            for(auto x : residues[i]) cout << x << endl;
            f.open(resDir + to_string(i));
            for(auto x : range(frequencies[i].size())) f << frequenciesBare[i][x] << "," << frequencies[i][x] << "," << weights[i][x] << "," << residues[i][x] << endl;
            f.close(); 
        }
    }

    void
    makeDirs()
    {
        dataDir += input.getString("qfactor","");
        mkdtemp(stoc(dataDir));
        // dataDir += "/" + input.getString("nLanczos","");
        // mkdtemp(stoc(dataDir));
        dataDir += "/";
        
        resDir += input.getString("qfactor","");
        mkdtemp(stoc(resDir));
        resDir += "/weights";
        mkdtemp(stoc(resDir));
        resDir += "/";
    }

    bool
    converged()
    {
        // TODO: Implement me
        return false;
    }




};
#endif