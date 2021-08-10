// #ifndef __CORRELATIONS_H_
// #define __CORRELATIONS_H_

// #include "static/static.h"
// #include "service/service.h"
// #include "measurement/measurement.h"
// #include "operator/operator.h"
// #include "state/statebuilder.h"

// using namespace itensor;

// class StaticCorrelation : public Static
// {
// protected:

// 	State psi;
// 	Lattice* lattice;
// 	Measurement* measurementx;
// 	Measurement* measurementq;
// 	Position* Sx;
// 	Momentum* Sq;
// 	vector<vector<StringReal>> resx;
// 	vector<StringReal> resp;
// 	// vector<StringReal> respc;
// 	vector<Real> qxs;
// 	vector<Real> qys;
// 	vector<Real> qzs;
// 	bool xBool = true;

	
// public:

// 	StaticCorrelation(Args* a, Lattice* l, Measurement* msx, Measurement* msq, Operator* ox, Operator* oq) : Static(a), lattice(l), measurementx(msx), measurementq(msq), Sx((Position*)ox), Sq((Momentum*)oq) {}
// 	StaticCorrelation(Args* a, Lattice* l, Measurement* msx, Measurement* msq, State* s, Operator* ox, Operator* oq) : Static(a), lattice(l), measurementx(msx), measurementq(msq), Sx((Position*)ox), Sq((Momentum*)oq)
// 	{
// 		psi = *s;
// 		_set_qs();
// 	}

// 	virtual void calculate()
// 	{
// 		// Position
// 		cout << "calculating position" << endl;
// 		resx.clear();
// 		for(auto s : lattice->getSites())
//         {
//             if(s.t == Lattice::physical)
//             {
//             	cout << s.s << endl;
//          		Sx->updatePosition(s.s);
//          		auto Spsi = Sx->multiply(psi);
//          		resx.push_back(measurementx->measure(psi,Spsi)); // measure <psi | S_i |Spsi> for all physical i.

//             }    
//         }
// 		// Momentum	

// 		cout << "calculating momentum" << endl;
// 		resp.clear();
// 		for(auto i : range(qxs.size()))
// 		{
// 			Sq->updateMomentum(qxs[i], qys[i], qzs[i]);
// 			auto Spsi = Sq->multiply(psi);
// 			auto Sq_exp = inner(psi.getState(),Spsi.getState());
// 			cout << qxs[i] << "," << qys[i] << "," << qzs[i] << "," << Sq_exp << "," << inner(Spsi.getState(),Spsi.getState()) << endl;
// 			resp.push_back(measurementq->measure(psi,Spsi)[0].real());

// 		}
// 	}

// 	static string getHash(Args* args)
// 	{	
// 		vector<string> strings{"SiteSet","Lattice","Model"};
// 		vector<string> reals{"N","MaxDim"};
// 		return "Correlations" + hash_real(reals, args) + hash_string(strings, args);
// 	}

// 	virtual void load(ifstream & f)
//     {
//     	measurementx->read(f);
//     	measurementq->read(f);
//     	Sx->read(f);
//     	Sq->read(f);
//     	read(f,psi);
//     	read(f,resx);
//     	read(f,resp);
//     	_set_qs();
//     }

// 	virtual void save(ofstream & f)
// 	{ 
// 		measurementx->write(f);
// 		measurementq->write(f);
// 		Sx->write(f);
// 		Sq->write(f);
// 		write(f,psi);
// 		write(f,resx);
// 		write(f,resp);
// 	}

// 	virtual tuple<vector<string>, vector<vector<StringReal>> > getResults()
// 	{	
// 		if(xBool)
// 		{
// 			xBool = false;
// 			return getXResults();
// 		}
// 		else
// 		{
// 			return getPResults();
// 		}
// 	}

// 	virtual vector<string> _labels(){ return vector<string>(); }
// 	virtual vector<vector<StringReal>> _results(){ return vector<vector<StringReal>>(); }

// 	virtual tuple<vector<string>, vector<vector<StringReal>> > getXResults()
// 	{	
// 		auto labels = _xlabels();
// 		auto results = _xresults();
// 		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
// 	}

// 	virtual tuple<vector<string>, vector<vector<StringReal>> > getPResults()
// 	{	
// 		auto labels = _plabels();
// 		auto results = _presults();
// 		return tuple<vector<string>, vector<vector<StringReal>> >(labels,results);
// 	}

// 	virtual vector<string> _plabels()
// 	{
// 		vector<string> labels{"S","qx","qy","qz","N","SiteSet","Lattice","Model","MaxDim","Geometry"};	
// 		if(args->getString("Lattice") == "Triangular") { labels.push_back("Nx"); labels.push_back("Ny"); }
// 		return labels;
// 	}

// 	virtual vector<string> _xlabels()
// 	{
// 		vector<string> labels{"S","i","j","rix","riy","riz","rjx","rjy","rjz","N","SiteSet","Lattice","Model","MaxDim","Geometry"};
// 		if(args->getString("Lattice") == "Triangular") { labels.push_back("Nx"); labels.push_back("Ny"); }
// 		return labels;
// 	}

// 	virtual vector<vector<StringReal>> _xresults()
// 	{
// 		auto results = vector<vector<StringReal>>();
// 		for(auto si : lattice->getSites())
// 		{
// 			for(auto sj : lattice->getSites())
// 			{
// 				if((si.t == Lattice::physical) && (sj.t == Lattice::physical))
// 				{
// 					vector<StringReal> temp;
// 					temp.push_back(resx[si.s-1][sj.s-1].real());
// 					temp.push_back(si.s);
// 					temp.push_back(sj.s);
// 					temp.push_back(si.rx);
// 					temp.push_back(si.ry);
// 					temp.push_back(si.rz);
// 					temp.push_back(sj.rx);
// 					temp.push_back(sj.ry);
// 					temp.push_back(sj.rz);
// 					temp.push_back(args->getReal("N"));
// 					temp.push_back(args->getString("SiteSet"));
// 					temp.push_back(args->getString("Lattice"));
// 					temp.push_back(args->getString("Model"));
// 					temp.push_back(args->getReal("MaxDim"));
// 					temp.push_back(args->getString("Geometry"));
// 					if(args->getString("Lattice") == "Triangular") { temp.push_back(args->getInt("Nx")); temp.push_back(args->getInt("Ny")); }
// 					results.push_back(temp);
// 				}
// 			}
// 		}
// 		return results;
// 	}

// 	virtual vector<vector<StringReal>> _presults()
// 	{
// 		auto results = vector<vector<StringReal>>();
// 		for(auto i : range(resp.size()))
// 		{
// 			vector<StringReal> temp;
// 			temp.push_back(resp[i]);
// 			temp.push_back(qxs[i]);
// 			temp.push_back(qys[i]);
// 			temp.push_back(qzs[i]);
// 			temp.push_back(args->getReal("N"));
// 			temp.push_back(args->getString("SiteSet"));
// 			temp.push_back(args->getString("Lattice"));
// 			temp.push_back(args->getString("Model"));
// 			temp.push_back(args->getReal("MaxDim"));
// 			temp.push_back(args->getString("Geometry"));
// 			if(args->getString("Lattice") == "Triangular") { temp.push_back(args->getInt("Nx")); temp.push_back(args->getInt("Ny")); }
// 			results.push_back(temp);
// 		}
// 		return results;
// 	}

// private:

// 	void _set_qs()
// 	{
// 		qxs = stringToVector(args->getString("qxs"));
// 		qys = stringToVector(args->getString("qys"));
// 		qzs = stringToVector(args->getString("qzs"));
// 		if(qys.size() == 0)
// 			for(auto qx : qxs) qys.push_back(0);
// 		if(qzs.size() == 0)
// 			for(auto qx : qxs) qzs.push_back(0);
// 		if (!(qxs.size() == qys.size()))
// 			Error("qxs and qys must be the same size.");
// 	}

// };
// #endif


