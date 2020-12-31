#ifndef __MAGNETIZATION_H_
#define __MAGNETIZATION_H_

#include "static/static.h"
#include "service/service.h"
#include "measurement/measurement.h"
#include "state/statebuilder.h"

using namespace itensor;

class Magnetization : public Static
{
protected:

	StateBuilder* stateBuilder;
	MeasurementBuilder* measurementBuilder;
	vector<vector<StringReal>> res;
	vector<Real> Bs;
	vector<StringReal> betas;

	
public:

	Magnetization(Args* a, StateBuilder* sb, MeasurementBuilder* msb) : Static(a), stateBuilder(sb), measurementBuilder(msb) 
	{
		auto BMin = args->getReal("BMin"); auto BMax = args->getReal("BMax"); auto BStep = args->getReal("BStep");
		auto betaMin = args->getReal("betaMin"); auto betaMax = args->getReal("betaMax"); auto betaStep = args->getReal("betaStep");
		for(Real B = BMin; B <= BMax; B += BStep) { Bs.push_back(B); }
		if(args->getBool("thermal")) for(Real beta = betaMin; beta <= betaMax; beta += betaStep) { betas.push_back(beta); } 
		else betas.push_back(string("infty"));
	}
	virtual void calculate()
	{
		auto _beta = args->getReal("beta"); auto _B = args->getReal("B"); // save value of B,beta.
		auto thermal = args->getBool("thermal"); 
		auto measurement = measurementBuilder->build(args);
		if(thermal)
		{
			for(auto &beta : betas)
			{
				cout << "beta = " << beta  << endl;
				args->add("beta",beta.real());
				vector<StringReal> temp;
				for(auto &B : Bs)
				{
					cout << "B = " << B << endl;
					args->add("B",B);
					auto state = stateBuilder->build(args);
					auto resx = measurement->measure(*state);
					Real _res = 0;
					for(auto &x : resx) _res += x.real(); // May want to generalize to complex?
					temp.push_back(_res);
				}
				res.push_back(temp);
			}	
		}
		else
		{
			args->add("beta","infty");
			vector<StringReal> temp;
			for(auto &B : Bs)
			{
				cout << "B = " << B << endl;
				args->add("B",B);
				auto state = stateBuilder->build(args);
				auto resx = measurement->measure(*state);
				Real _res = 0;
				for(auto &x : resx) _res += x.real(); // May want to generalize to complex?
				temp.push_back(_res);
			}
			res.push_back(temp);
		}
		args->add("beta",_beta); args->add("B",_B); // restore value of B,beta.
	}

	static string getHash(Args* args)
	{	
		vector<string> strings{"SiteSet","Lattice","Model"};
		vector<string> reals{"N","MaxDim","beta-tau"};
		return "Magnetization" + hash_real(reals, args) + hash_string(strings, args);
	}

	virtual vector<string> _labels()
	{
		vector<string> labels{"M","beta","B","N","SiteSet","Lattice","Model","MaxDim","beta-tau"};
		return labels;
	}

	virtual vector<vector<StringReal>> _results()
	{
		auto results = vector<vector<StringReal>>();
		for(int ibeta : range(res.size()))
		{	
			for(int iB : range(res[ibeta].size()))
			{
				vector<StringReal> temp;
				temp.push_back(res[ibeta][iB].real());
				( (args->getBool("thermal")) ? temp.push_back(betas[ibeta].real()) : temp.push_back(string("infty")) );
				temp.push_back(Bs[iB]);
				temp.push_back(args->getReal("N"));
				temp.push_back(args->getString("SiteSet"));
				temp.push_back(args->getString("Lattice"));
				temp.push_back(args->getString("Model"));
				temp.push_back(args->getReal("MaxDim"));
				temp.push_back(args->getReal("beta-tau"));
				results.push_back(temp);
			}
			
		}
		return results;
	}


};
#endif