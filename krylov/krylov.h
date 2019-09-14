#ifndef __KRYLOV_H_
#define __KRYLOV_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
#include "infrastructure/util.h"

using namespace itensor;
using namespace std;

class Krylov
{
public:
	class Matrices
	{
	public:
		CMatrix t;
		Matrices(CMatrix& _t) {t = t;}
	};
protected:
	/* Inputs */
	Args* args;
	Model* model;
	State* state;
	MPS psii;
	MPO H;

	/* Helpers */
	int i;
	int maxIter;
	vector<MPS> V;

	/* Outputs */
	int iterations;
	CMatrix T;
	Real E0;

public:
	Krylov(){}
	Krylov(Args* a, Model* m, State* s)
	{ 

		args = a;
		model = m;
		state = s;
		psii = state->getState();
		E0 = state->getE0();
		H = model->getH();
		calcKrylov();	
	}
	virtual ~Krylov(){}
	virtual CMatrix getT() {return T; }
	virtual vector<CMatrix> getTs() {return vector<CMatrix>{T}; }
	virtual Matrices* getMatrices() {return new Matrices(T); }
	virtual int getIterations() {return iterations; }
	int getE0() {return E0; }
	static string getHash(Args* args)
	{
		return State::getHash(args) + "_" + to_string(args->getInt("nLanczos"));
	}

	virtual void load(ifstream & f)
	{
		read(f,iterations);
		read(f,T);
		read(f,E0);
	}
	virtual void save(ofstream & f)
	{
		write(f,iterations);
		write(f,T);
		write(f,E0);
	}






protected:
	void calcKrylov()
	{
		/* Calc Hred, you already have this code, just paste and modify.*/
		maxIter = args->getInt("nLanczos");
		auto is = siteInds(psii);
		auto c = args->getInt("c");

        psii.position(c);
        H.position(c);
        V = std::vector<MPS>(maxIter,MPS(psii));
        auto W = std::vector<MPS>(maxIter,MPS(psii));
        auto WP = std::vector<MPS>(maxIter,MPS(psii));
        T = CMatrix(maxIter,maxIter);
        for(auto& el : T) el = Cplx(0,0);
        V[0] = MPS(psii);
       	WP[0] = applyMPO(H,V[0],args);
        T(0,0) = innerC(WP[0],V[0]).real();
        prepare(V[0],WP[0],c,is);
        W[0] = sum(WP[0],-1*T(0,0)*V[0],args);
        i = 1;
        while(!converged())
        {
        	cout << i << endl;
            T(i-1,i) = norm(W[i-1]);
            T(i,i-1) = norm(W[i-1]);
            V[i] = (1.0/norm(W[i-1]))*W[i-1];
            WP[i] = applyMPO(H,V[i],args);
            T(i,i) = innerC(WP[i],V[i]).real();
            prepare(WP[i],V[i],c,is);
            auto temp = sum(WP[i],-1*T(i,i)*V[i],args);
            prepare(temp,V[i-1],c,is);
            W[i] = sum(temp,-1*T(i,i-1)*V[i-1],args);
            i+=1;
        }
        iterations = i;
        if(iterations < maxIter) T = subMatrix(T,0,iterations,0,iterations);
	}

	virtual bool converged()
	{
		if(i >= maxIter) return true;
		else return false;
	}
	prepare(MPS &a, MPS &b, int c)
	{
	    a.replaceSiteInds(is);
	    b.replaceSiteInds(is);
	    a.position(c);
	    b.position(c);
	}

};

#endif
