#ifndef __MODEL_H_
#define __MODEL_H_

#include <string>
#include <iostream>
#include "itensor/all.h"
// #include "heisenbergchain.h"
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
protected:
	MPO H;
	MPS psi0; // Ground state.
	MPS psii;
	Real E0;
	int N;
	bool readIn = false;
	InputGroup input;
	SiteSet sites;
	string dataDir;
	string resDir;

public:
	Model() {}
	Model(InputGroup i) : input(i) {}
	virtual void build() = 0;
	virtual void setPsii() = 0;
	void setPsi0()
	{
		if(input.getYesNo("Thermal",0))
			thermal();
		else
			groundState();
	}
	virtual void thermal()
	{
		cout << "Thermal is not implemented yet." << endl;
		exit(0);
	}
	virtual void groundState()
	{
		if(readIn) return;
		auto nsweeps = input.getInt("nsweeps",5);
        auto table = InputGroup(input,"sweeps");
        auto sweeps = Sweeps(nsweeps,table);
        auto [e,p] = dmrg(H,psii,sweeps,"Quiet");
        E0 = e;
        psi0 = p;
        write();
	}

	virtual bool
	read()
	{
		if(pathExists(dataDir+"H"))
		{
	        H = readFromFile<MPO>(dataDir+"H");
	        psi0 = readFromFile<MPS>(dataDir+"psi0");
	        E0 = readFromFile<Real>(dataDir+"E0");
	        sites = readFromFile<SpinHalf>(dataDir+"sites");
	        N = psi0.length();
	        return true;
    	}
    	return false;
	}

	virtual void
	write()
	{
		writeToFile(dataDir+"psi0",psi0);
        writeToFile(dataDir+"H",H);
        writeToFile(dataDir+"E0",E0);
        writeToFile(dataDir+"sites",sites);
	}

	virtual void
	makeDirs()
	{
	    auto model = input.getString("model","Heisenberg");
	    auto lattice = input.getString("lattice","chain"); // additional option is bondList
	    auto cwd = input.getString("cwd","");
		N = input.getInt("N",100);
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

	MPO
	getH(){ return H; }
	MPS
	getPsi0(){ return psi0; }
	string
	getDataDir(){ return dataDir; }
	string
	getResDir(){ return resDir; }
	SiteSet
	getSites(){ return sites; }
	Real
	getE0(){ return E0; }

};
#endif
