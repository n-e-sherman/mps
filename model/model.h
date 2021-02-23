#ifndef __MODEL_H_
#define __MODEL_H_



#include "lattice/lattice.h"
#include "operator/operator.h"
#include <string>
#include <iostream>
#include <cmath>
#include <string>
#include <typeinfo>
#include <optional>

using namespace itensor;
using namespace std;

class Model : public Operator
{
public:
	struct gate
	{ 
		int s1 = 0; int s2 = 0; ITensor t; string l = "";
	};
protected:

	/* Inputs */
	Lattice* lattice;

	/* Outputs */
	map<string, Real> params;

	virtual AutoMPO calcAmpoH(bool skip = false, Real shift = 0) = 0;
	virtual AutoMPO calcAmpoL(bool skip = false, Real shift = 0) = 0;
	virtual vector<gate> calcGatesH() = 0;
	virtual vector<gate> calcGatesL() = 0;
	virtual void calcAmpoShift(AutoMPO& ampo, Real shift)
	{ 
		for(auto s : lattice->getSites()){ if(s.t == Lattice::physical) { ampo +=   shift,"Id",s.s; } } 
	}
	

public:
	Model(Args* a, Lattice* l, Sites* s) : Operator(a,s), lattice(l) {}

	virtual void calcH(bool skip = false, Real scale = 1.0, Real shift = 0){ O = scale*toMPO(calcAmpoH(skip,shift),*args); }
	virtual void calcL(bool skip = false, Real scale = 1.0, Real shift = 0){ O = scale*toMPO(calcAmpoL(skip,shift),*args); }

	virtual void calcExpH(Cplx tau){ O = toExpH(calcAmpoH(), tau, *args); }
	virtual void calcExpL(Cplx tau){ O = toExpH(calcAmpoL(), tau, *args); }

	vector<gate> getGatesH() { return calcGatesH(); }
	vector<gate> getGatesL() { return calcGatesL(); }
	map<string, Real> getParams() {return params; }
	Sites* getSites() const {return sites; }

	/* ITensor Wrappers */
	void orthogonalize(){ O.orthogonalize(*args); }
	int maxLinkDim() { return itensor::maxLinkDim(O); }
	Real averageLinkDim() { return itensor::averageLinkDim(O); }

	static string hashParams(Args* args)
	{ // A better way to do this?
		string res = "";
		auto sModel = args->getString("Model");
		if(sModel == "XXZ")
			res = "_" + to_string(args->getReal("Jxy")) + to_string(args->getReal("Delta"));
		if(sModel == "Dimer")
			res = "_" + to_string(args->getReal("Je")) + to_string(args->getReal("Jo"));
		if(sModel == "HeisenbergField")
			res = "_" + to_string(args->getReal("B"));
		return res;
	}

	void read(istream& is)
    {
    	Operator::read(is);
    	vector<string> keys;
    	vector<Real> values;
    	itensor::read(is,keys);
    	itensor::read(is,values);
    	params = vtod(keys,values);
    }
    void write(ostream& os) const
    {
    	Operator::write(os);
    	auto [keys,values] = dtov(params);
        itensor::write(os,keys);
        itensor::write(os,values);
    }



private:
	tuple<vector<string>, vector<Real> > dtov(map<string, Real> d) const
	{	
		vector<string> keys;
		vector<Real> values;
		for(auto& x : params){ keys.push_back(x.first); values.push_back(x.second); }
		return tuple< vector<string>, vector<Real> >(keys,values);
	}

	map<string,Real> vtod(vector<string> keys, vector<Real> values) // Could be made a part of a "service" which is inherited by this and Correlation
	{	
		map<string,Real> res;
		for(int i : range(keys.size()))
		{
			res[keys[i]] = values[i];
		}
		return res;
	}



	
};

#endif
