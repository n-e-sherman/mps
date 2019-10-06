#ifndef __REORTHOGONALIZE_H_
#define __REORTHOGONALIZE_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "infrastructure/util.h"

using namespace itensor;
using namespace std;

class Reorthogonalize : public Lanczos
{
protected:
	/* Krylov makes T,V,iterations and has H and psi*/

	/* Helper */
	CMatrix W; 
	CMatrix K; 
	CMatrix ST;
	CVector Norm;
    Real eps;

	/* Output */
    CMatrix S;
    CMatrix HP;
    CMatrix HP2; 
	CMatrix TR;

public:
	Reorthogonalize(){}
	Reorthogonalize(Args* a, Model* m, State* s) : Lanczos(a,m,s)
	{ 
        eps = args->getReal("eps");
		reorthogonalize();

	}
	virtual ~Reorthogonalize(){}
    virtual CMatrix getT() {return TR; }
    virtual vector<CMatrix> getTs() {return vector<CMatrix>{T,TR}; }
    virtual Matrices* getMatrices() 
    {
        return new Matrices(T,TR,S,HP,HP2);
    }
    virtual void load(ifstream & f)
    {
        Lanczos::load(f);
        read(f,S);
        read(f,HP);
        read(f,HP2);
        read(f,TR);
    }
    virtual void save(ofstream & f)
    {
        Lanczos::save(f);
        write(f,S);
        write(f,HP);
        write(f,HP2);
        write(f,TR);
    }

	
protected:
	void reorthogonalize()
	{
        HP = CMatrix(iterations,iterations);
        for (auto &el : HP) el = Cplx(NAN,NAN);
        HP2 = CMatrix(iterations,iterations);
        for (auto &el : HP2) el = Cplx(NAN,NAN);
        W  = CMatrix(iterations,iterations);
        for (auto &el : W) el = Cplx(NAN,NAN);
        K  = CMatrix(iterations,iterations);
        for (auto &el : K) el = Cplx(NAN,NAN);
        ST = CMatrix(iterations,iterations);
        for (auto &el : ST) el = Cplx(NAN,NAN);
        S  = CMatrix(iterations,iterations);
        for (auto &el : S) el = Cplx(NAN,NAN);
        TR = CMatrix(iterations,iterations);
        for (auto &el : TR) el = Cplx(0,0);
        Norm  = CVector(iterations);
        for (auto &el : Norm) el = Cplx(NAN,NAN);

        for(auto i : range(iterations))
        {
            cout << i << endl;
            for(auto j : range(i+1))
            {
                W(i,j) = innerC(V[i],V[j]);
                HP(i,j) = innerC(V[i],H,V[j]);          
                HP2(i,j) = innerC(prime(V[i]),prime(H),H,V[j]);
                W(j,i) = W(i,j);
                HP(j,i) = HP(i,j);          
                HP2(j,i) = HP2(i,j);
            }
        }
        S(0,0) = 1;
        Norm(0) = sqrt(W(0,0));
        for(auto i : range(iterations)) getS(i,iterations-1);

        for(auto &el : TR) el = Cplx(0,0);

        for(auto n : range(iterations))
        for(auto m : range(iterations))
        for(auto i: range(n+1))
        for(auto j: range(m+1))
            TR(n,m) += conj(S(i,n))*S(j,m)*HP(i,j);

    }

    Cplx
    getS(int i, int j)
    {   
        if(!isnan(S(i,j).real())) return S(i,j);

        Cplx val = Cplx(0,0);
        val = getST(i,j)/getN(j);
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
        if(Norm(j) == Cplx(0,0)) Norm(j) = Cplx(eps,0);
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


};

#endif
