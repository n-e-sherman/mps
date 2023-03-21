#ifndef __THERMALLADDER_H_
#define __THERMALLADDER_H_

#include "lattice/lattice.h"
#include "repository/repository.h"

using namespace std;
using namespace itensor;

class ThermalLadder : public Lattice
{
protected:

	LatticeGraph lat;
	vector<vector<Real>> positions;

public:
	

	ThermalLadder(Args* a) : Lattice(a) 
	{	
		auto Nx = args->getInt("Nx");
		auto Ny = 2;
		auto N = Nx*Ny;
		if(N != args->getInt("N")) { cout << "N is not equal to Nx*Ny, Modifying this." << endl; args->add("N",N); }

		/* lattice data */
		lat = squareNextNeighbor(Nx,Ny,*args);
		get_positions(Nx, Ny);



		/* bonds */
		for(auto &l : lat) 
		{
			int type = 2;
			if ((l.s1 % 2) == (l.s2 % 2)) type = 1;
			bonds.push_back(bond{2*l.s1-1,2*l.s2-1,physical,-1, type});
			bonds.push_back(bond{2*l.s1,2*l.s2,environment,-1, type});
		}
		for(int i : range1(N))
		{
			sites.push_back(site{2*i-1,positions[i-1][0],positions[i-1][1],positions[i-1][2],physical});	
			sites.push_back(site{2*i,positions[i-1][0],positions[i-1][1],positions[i-1][2],environment});	
		} 
	}

    void save_lattice(Repository* repo)
    {
    	auto labels = vector<string>({"x","y"});
    	auto Nx = args->getInt("Nx");
		auto Ny = args->getInt("Ny");
		auto N = Nx*Ny;
		vector<vector<Real>> _sites;
		vector<vector<int>> _bonds;
		vector<vector<int>> _seconds;
    	for(auto &b : bonds)
    	{
    		if (b.n == 1)
    			_bonds.push_back(vector<int>{b.s1,b.s2});
    		if (b.n == 2)
    			_seconds.push_back(vector<int>{b.s1,b.s2});
    	}
    	for(auto &s : sites)
    		_sites.push_back(vector<Real>{s.rx,s.ry});

    	repo->save("sites","temp",labels,_sites,true);
    	repo->save("bonds","temp",labels,_bonds,true);
    	repo->save("seconds","temp",labels,_seconds,true);
    }


protected:


	void get_positions(int Nx, int Ny)
	{
		auto N = Nx * Ny;
		for(int n = 1; n <= N; ++n)
        {
	        int x = (n-1)/Ny+1;
	        int y = (n-1)%Ny+1;
	        positions.push_back({Real(x),Real(y),0});
	    }
	}    
    
};

#endif