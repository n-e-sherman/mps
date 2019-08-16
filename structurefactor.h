#ifndef __STRUCTUREFACTOR_H_
#define __STRUCTUREFACTOR_H_

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
class StructureFactor : public Quantity
{
protected:

	CMatrix T;
    CMatrix TR;
	vector<MPS> V;
	Args args;
    MPS correction;
	MPS psii; // psii = S^z_c|psi0>
	int nMax;
    Real psiiNorm;
    Real spectralNorm;
    string tag;
    vector<string> setas;
    vector<Cplx> etas;
    vector<Real> omegas;
	CMatrix HP; 
	CMatrix W; 
	CMatrix K; 
	CMatrix ST;
	CMatrix S;
	CVector Norm;

public:

	StructureFactor() {}
	StructureFactor(Model* m,InputGroup i) : Quantity(m,i) {}


    void
    calculate()
    {
        set();
        makeDirs();
        setPsii();
        auto r = input.getYesNo("reortho",0);
        tag = "";
        if(r) tag = "_R"; 
        if(pathExists(dataDir+"T"+tag) & !input.getYesNo("calcT",0)) // Already have T
        {
            read();
            calcResult();
        }
        else // Need to calculate T
        {
            tag="";
            calcT();
            write();
            calcResult();
            if(r) 
            {
                tag="_R";
                reorthogonalize();
                write();
                calcResult();
            }
        }
    }

    void
    calcResult()
    {

        auto qfactor = input.getReal("qfactor",1); // q = qfactor * pi * N/(N+1)
        auto S = calcSpectral();
        writeResult(S);
    }

    vector<vector<Real>>
    calcSpectral() 
    {
        // output will be S[eta][omega]
        Vector d;
        CMatrix U;
        auto Tref = subMatrix(T,0,nMax,0,nMax);
        diagHermitian(Tref,U,d);
        Print(d);
        vector<vector<Real>> result;
        for(auto eta : etas)
        {
            vector<Real> temp;
            for(auto omega : omegas)
            {
                auto z = omega + E0 + eta;
                auto delta = CVector(nMax);
                for(int i = 0; i < nMax; i++) delta(i) = 1.0/(z-d(i));
                auto D = CMatrix(nMax,nMax);
                diagonal(D) &= delta;
                auto R = U*D*conj(transpose(U)); // 1/(z-M)
                auto res = R(0,0).imag();
                res = -1*(1.0/M_PI)*psiiNorm*psiiNorm*res;
                temp.push_back(res);
            }
            result.push_back(temp);
        }
        return result;
    }

	void
	calcT()
    {
    	auto maxIter = input.getInt("nLanczos",50);
        psii.position(c);
        H.position(c);
        V = std::vector<MPS>(maxIter+2,MPS(psii));
        auto W = std::vector<MPS>(maxIter+2,MPS(psii));
        auto WP = std::vector<MPS>(maxIter+2,MPS(psii));
        T = CMatrix(maxIter+2,maxIter+2);
        for(auto& el : T) el = Cplx(0,0);
        V[0] = MPS(psii);
       	WP[0] = applyMPO(H,V[0],args);
        T(0,0) = innerC(WP[0],V[0]).real();
        prepare(V[0],WP[0]);
        W[0] = sum(WP[0],-1*T(0,0)*V[0],args);
        int i = 1;
        while((i < maxIter) && !converged())
        {
        	cout << i << endl;
            T(i-1,i) = norm(W[i-1]);
            T(i,i-1) = norm(W[i-1]);
            V[i] = (1.0/norm(W[i-1]))*W[i-1];
            WP[i] = applyMPO(H,V[i],args);
            T(i,i) = innerC(WP[i],V[i]).real();
            prepare(WP[i],V[i]);
            auto temp = sum(WP[i],-1*T(i,i)*V[i],args);
            prepare(temp,V[i-1]);
            W[i] = sum(temp,-1*T(i,i-1)*V[i-1],args);
            i+=1;
        }
        nMax = i;
    }

    void
    reorthogonalize()
    {
        cout << "Reorthogonalizing T." << endl;
        HP = CMatrix(nMax,nMax);
        for (auto &el : HP) el = Cplx(NAN,NAN);
        W  = CMatrix(nMax,nMax);
        for (auto &el : W) el = Cplx(NAN,NAN);
        K  = CMatrix(nMax,nMax);
        for (auto &el : K) el = Cplx(NAN,NAN);
        ST = CMatrix(nMax,nMax);
        for (auto &el : ST) el = Cplx(NAN,NAN);
        S  = CMatrix(nMax,nMax);
        for (auto &el : S) el = Cplx(NAN,NAN);
        Norm  = CVector(nMax);
        for (auto &el : Norm) el = Cplx(NAN,NAN);


        cout << "Creating W and HP." << endl;
        for(auto i : range(nMax))
        {
            cout << i << endl;
            for(auto j : range(nMax))
            {
                W(i,j) = innerC(V[i],V[j]);
                HP(i,j) = innerC(V[i],H,V[j]);
            }
        }
        S(0,0) = 1;
        Norm(0) = sqrt(W(0,0));
        // getS(nMax-1,nMax-1);
        for(auto n : range(nMax))
        for(auto i : range(n+1)) {cout << "Getting S : " << i << "," << n << endl; getS(i,n);}
        // for(auto i =nMax-1; i >= nMax/2; i--) getS(i,nMax-1);
        cout << "built S" << endl;
        Print(N);

        for(auto &el : T) el = Cplx(0,0);

        for(auto n : range(nMax))
        for(auto m : range(nMax))
        for(auto i: range(n+1))
        for(auto j: range(m+1))
            T(n,m) += conj(S(i,n))*S(j,m)*HP(i,j);

        cout << "reortho done." << endl;
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
    // Cplx
    // getS(int i, int j)
    // {   
    //     if(!isnan(S(i,j).real())) return S(i,j);

    //     Cplx val = Cplx(0,0);
    //     val = getST(i,j)/getN(j); // WAS THIS A BUG!?!?!?
    //     S(i,j) = val;
    //     return val;
    // }

    // Cplx
    // getST(int i, int j)
    // {
    //     if(!isnan(ST(i,j).real())) return ST(i,j);

    //     if(i == j)
    //     {
    //         ST(i,j) = Cplx(1,0);
    //         return Cplx(1,0); 
    //     }

    //     Cplx val = Cplx(0,0);
    //     for(auto k : range(i,j)) val -= getK(k,j)*getS(i,k);
    //     ST(i,j) = val;
    //     return val;
    // }

    // Cplx
    // getN(int j)
    // {
    //     if(!isnan(Norm(j).real())) return Norm(j);

    //     Cplx val = Cplx(0,0);
    //     for(auto k : range(j+1))
    //     for(auto l : range(j+1))
    //         val += getST(k,j)*getST(l,j)*W(k,l);
    //     Norm(j) = sqrt(val);
    //     return Norm(j);
    // }

    // Cplx
    // getK(int i, int j)
    // {
    //     if(!isnan(K(i,j).real())) return K(i,j);

    //     Cplx val = Cplx(0,0);
    //     for(auto k : range(i+1)) val += getS(k,i)*W(j,k);
    //     K(i,j) = val;
    //     return val;
    // }

    vector<Real>
    getOmegas()
    {
        auto wi = input.getReal("wi",0);
        auto wf = input.getReal("wf",3);
        auto n  = input.getInt("nw",31);
        auto omegas = linspace(wi,wf,n);
        return omegas;
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
        auto temp = input.getString("etas","");
        setas = splitString(temp);
        auto retas = stringToVector(temp);
        for(auto x : retas) etas.push_back(Cplx(0,x));
        omegas = getOmegas();
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
	read()
	{
        T = readFromFile<CMatrix>(dataDir+"T"+tag);
        nMax = readFromFile<int>(dataDir+"nMax");
        psiiNorm = readFromFile<Real>(dataDir+"psiiNorm");
	}

	void
	write()
	{
		writeToFile(dataDir+"T"+tag,T);
        writeToFile(dataDir+"nMax",nMax);
        writeToFile(dataDir+"psiiNorm",psiiNorm);
	}

    void
    writeResult(vector<vector<Real>> &res)
    {
        auto wi = omegas.front();
        auto wf = omegas.back();
        auto n = omegas.size();
        ofstream f;
        for(auto i : range(setas.size()))
            {
            auto resPath = resDir + setas[i] + "/" + "S" + tag;
            f.open(resPath);
            for(auto j : range(omegas.size()))
            {
                f << omegas[j] << "," << res[i][j] << endl;
            }
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
        resDir += "/SF" ;
        mkdtemp(stoc(resDir));
        resDir += "/";
        for(auto x : setas)
        {
            auto temp = resDir + x;
            mkdtemp(stoc(temp));
        }
    }

    bool
    converged()
    {
    	// TODO: Implement me
    	return false;
    }


};
#endif