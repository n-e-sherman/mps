#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
// #include "krylov.h"
#include "util.h"
#include <cmath>

using namespace itensor;
using namespace std;

// This class will initialize the Hamiltonian and get site
// structure for subsequent calculations. Will also find the
// ground state of the Hamiltonian.
// read&write will follow the naming convention
// "modelname_lattice_systemsize_quantity"
class Model
{
	MPO H;
	MPS psi0; // Ground state.
	Real E0;
	int N;
	string dataDir;
	string resDir;
	string model;
	string lattice;
	SpinHalf sites;
	
    InputGroup input;
	string cwd; // ends in a forward slash /
	vector<tuple<int,int>> bonds;
	MPS psii; // Initial guess.

	public:

	Model() {}
	Model(InputGroup i) : input(i) {}

	bool
	build()
	{
		makeDirs();
		if(pathExists(dataDir+"H") & !input.getYesNo("dmrg",0))
		{
			read();
		}
		else
		{
			if(lattice == "bondList") 
			{
				if(!bondList()) return false;
			}
			else if(lattice == "chain") chain();
			else
			{
				cout << "Specified lattice is not implemented." << endl;
				return false;
			}

			if(model == "Heisenberg") heisenberg();
			else
			{
				cout << "Specified model is not implemented." << endl;
				return false;
			}
			groundState();
			write();
		}
		return true;


	}

	bool
	bondList()
	{
		auto bondsLoc = input.getString("bondsLoc","bonds");
		if(pathExists(cwd+bondsLoc))
		{
			// TODO: implement me
			cout << "Bonds file is not implemented yet." << endl;
			return false;
		}
		else
		{
			cout << "Could not find bonds location." << endl;
			return false;
		}
	}

	void
	chain()
	{
		for(auto i : range1(N-1)) bonds.push_back(make_tuple(i,i+1));
	}

	void
	heisenberg()
	{
		sites = SpinHalf(N);
        auto ampo = AutoMPO(sites);
        for(auto b : bonds)
        {
            ampo += 0.5,"S+",get<0>(b),"S-",get<1>(b);
            ampo += 0.5,"S-",get<0>(b),"S+",get<1>(b);
            ampo +=     "Sz",get<0>(b),"Sz",get<1>(b);
        }
        H = toMPO(ampo);
        auto state = InitState(sites);
        for(auto i : range1(N))
            {
            if(i%2 == 1) state.set(i,"Up");
            else         state.set(i,"Dn");
            }
        psii = MPS(state);
	}

	void
	groundState()
	{
		auto nsweeps = input.getInt("nsweeps",5);
        auto table = InputGroup(input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table);
        auto [e,p] = dmrg(H,psii,sweeps,"Quiet");
        E0 = e;
        psi0 = p;
	}

	Real
	set(MPO& h, MPS& p)
	{
		// h = MPO(H);
		// p = new MPS(psi0);
		// h = &H;
		// p = &psi0;
		return E0;
	}

	MPO
	getH()
	{
		return H;
	}

	MPS
	getPsi0()
	{
		return psi0;
	}

	string
	getDataDir()
	{
		return dataDir;
	}

	string
	getResDir()
	{
		return resDir;
	}

	SpinHalf
	getSites()
	{
		return sites;
	}

	Real
	getE0()
	{
		return E0;
	}

	void
	read()
	{
        H = readFromFile<MPO>(dataDir+"H");
        psi0 = readFromFile<MPS>(dataDir+"psi0");
        E0 = readFromFile<Real>(dataDir+"E0");
        dataDir = readFromFile<string>(dataDir+"dataDir");
        resDir = readFromFile<string>(dataDir+"resDir");
        sites = readFromFile<SpinHalf>(dataDir+"sites");
        N = psi0.length();
	}

	void
	write()
	{
		writeToFile(dataDir+"psi0",psi0);
        writeToFile(dataDir+"H",H);
        writeToFile(dataDir+"E0",E0);
        writeToFile(dataDir+"dataDir",dataDir);
        writeToFile(dataDir+"resDir",resDir);
        writeToFile(dataDir+"sites",sites);
	}

	void
	makeDirs()
	{
	    model = input.getString("model","Heisenberg");
	    lattice = input.getString("lattice","chain"); // additional option is bondList
	    cwd = input.getString("cwd","");
		N = input.getInt("N",N);
	    mkdtemp(stoc(cwd + "data"));
	    cout << "dataDirs: " << cwd + "data" << endl;
	    mkdtemp(stoc(cwd + "results"));
	    cout << "dataDirs: " << cwd + "results" << endl;
	    dataDir = cwd+"data/"+model+"_"+lattice+"_"+std::to_string(N);
	    mkdtemp(stoc(dataDir));
	    dataDir += "/"+input.getString("maxDim","");
	    mkdtemp(stoc(dataDir));
	    dataDir += "/";


	    resDir = cwd+"results/"+model+"_"+lattice+"_"+std::to_string(N);
	    mkdtemp(stoc(resDir));
	    resDir += "/"+input.getString("maxDim","");
	    mkdtemp(stoc(resDir));
	    resDir += "/";
	}


};
#endif
