#ifndef __TRIANGULAR_H_
#define __TRIANGULAR_H_

#include "lattice/lattice.h"

using namespace std;
using namespace itensor;

class Triangular : public Lattice
{
protected:

	vector<vector<int>> adjmat; // Is this needed as a member?
	vector<vector<int>> triangles;
	vector<vector<int>> seconds;
	vector<vector<int>> thirds;

public:
	

	Triangular(Args* a) : Lattice(a) 
	{	
		auto Nx = args->getInt("Nx");
		auto Ny = args->getInt("Ny");
		auto N = Nx*Ny;
		if(N != args->getInt("N")) { cout << "N is not equal to Nx*Ny, Modifying this." << endl; args->add("N",N); }

		/* lattice data */
		auto lat = triangularLattice(Nx,Ny,*args);
		get_adjmat(N,lat);
		get_triangles();
		get_seconds();
		get_thirds();

		/* bonds */
		for(auto &l : lat) bonds.push_back(bond{l.s1,l.s2,physical,-1,1});  // z seems ill-defined here?
		for(auto &b : seconds) bonds.push_back(bond{b[0],b[1],physical,-1,2});  // z seems ill-defined here?
		for(auto &b : thirds) bonds.push_back(bond{b[0],b[1],physical,-1,3});  // z seems ill-defined here?

		/* sites */
		for(int i : range1(N)) sites.push_back(site{i,i,0,physical});
	}

protected:

    void get_adjmat(int N, LatticeGraph& lat)
    {
    	adjmat = vector<vector<int>>(N+1,vector<int>(N+1));
    	for(auto &l : lat)
	    {
	        adjmat[l.s1][l.s2] = 1;
	    }	
    }

	void get_triangles() // adjmat used
    {
    	for(int i : range(adjmat.size()))
	    {
	        vector<int> js; for(auto j : range(adjmat[i].size())) {if(adjmat[i][j] == 1) js.push_back(j); } // bonds from i
	        vector<vector<int>> kss; // bonds from j for each j
	        for(int j : js) 
	        {
	            vector<int> ks_temp;
	            for (auto k : range(adjmat[j].size())) {if(adjmat[j][k] == 1) ks_temp.push_back(k); }
	            kss.push_back(ks_temp);
	        }

	        for(int jInd : range(js.size()))
	        {
	            auto ks = kss[jInd];
	            if(ks.size() == 0)
	                continue;
	            for(auto k : ks)
	            {
	                if(std::find(js.begin(), js.end(), k) != js.end()) // We have a triangle
	                {
	                    triangles.push_back(vector<int>{i,js[jInd],k});   
	                }
	            }
	        }
	    }
    }
    
    void get_seconds() // adjmat used
    {
	    for(auto _tri : triangles)
	    {
	        set<int> tri; for(auto x : _tri) tri.insert(x);
	        for(auto _tri2 : triangles)
	        {
	            set<int> tri2; for(auto x : _tri2) tri2.insert(x);
	            set<int> tri_intersect;
	            set_intersection(tri.begin(), tri.end(), tri2.begin(), tri2.end(), std::inserter(tri_intersect, tri_intersect.begin()));
	            if(tri_intersect.size() == 2) // 2nd neighbor found
	            {
	                set<int> tri_union;
	                set_union(tri.begin(), tri.end(), tri2.begin(), tri2.end(), std::inserter(tri_union, tri_union.begin()));
	                set<int> _second;
	                set_difference(tri_union.begin(), tri_union.end(), tri_intersect.begin(), tri_intersect.end(), std::inserter(_second, _second.begin()));
	                vector<int> second (_second.begin(),_second.end());
	                adjmat[second[0]][second[1]] = 2;
	            }
	        }
	    }

	    seconds = vector<vector<int>>();
	    for(int i : range(adjmat.size()))
	    {
	    	for(int j : range(i+1,adjmat[i].size()))
	    	{
	    		if (adjmat[i][j] == 2)
	    		{
	    			seconds.push_back(vector<int>{i,j});
	    		}
	    	}
	    }
    }
    
    void get_thirds() // adjmat used
    {
	    for(int i : range(adjmat.size()))
	    {
	        vector<int> js; for(auto j : range(adjmat[i].size())) {if(adjmat[i][j] == 1) js.push_back(j); } // bonds from i
	        vector<vector<int>> kss; // bonds from j for each j
	        for(int j : js) 
	        {
	            vector<int> ks_temp;
	            for (auto k : range(adjmat[j].size())) {if(adjmat[j][k] == 1) ks_temp.push_back(k); }
	            kss.push_back(ks_temp);
	        }

	        for(int jInd : range(js.size()))
	        {
	            auto ks = kss[jInd];
	            if(ks.size() == 0)
	                continue;
	            for(auto k : ks)
	            {
	                if(adjmat[i][k] == 0)
	                {
	                    adjmat[i][k] = 3;
	                }
	            }
	        }
	    }

	    thirds = vector<vector<int>>();
	    for(int i : range(adjmat.size()))
	    {
	    	for(int j : range(i+1,adjmat[i].size()))
	    	{
	    		if (adjmat[i][j] == 3)
	    		{
	    			thirds.push_back(vector<int>{i,j});
	    		}
	    	}
	    }	
    }
    
};

#endif