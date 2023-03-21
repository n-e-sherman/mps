#ifndef __TRIANGULAR_H_
#define __TRIANGULAR_H_

#include "lattice/lattice.h"
#include "repository/repository.h"

using namespace std;
using namespace itensor;

class Triangular : public Lattice
{
protected:

	LatticeGraph lat;
	vector<vector<int>> adjmat; // Is this needed as a member?
	vector<vector<int>> triangles;
	vector<vector<int>> seconds;
	vector<vector<int>> thirds;
	vector<vector<int>> fourths_plus;
	vector<vector<int>> fourths_sub;
	vector<vector<Real>> positions;

public:
	

	Triangular(Args* a) : Lattice(a) 
	{	
		auto Nx = args->getInt("Nx");
		auto Ny = args->getInt("Ny");
		auto N = Nx*Ny;
		if(N != args->getInt("N")) { cout << "N is not equal to Nx*Ny, Modifying this." << endl; args->add("N",N); }

		/* lattice data */
		get_lattice(Nx,Ny);
		get_adjmat(N);
		// get_triangles();
		get_seconds(Nx,Ny);
		// get_thirds();
		// get_fourths();


		/* bonds */
		for(auto &l : lat) bonds.push_back(bond{l.s1,l.s2,physical,-1,1});  // z seems ill-defined here?
		for(auto &b : seconds) bonds.push_back(bond{b[0],b[1],physical,-1,2});  // z seems ill-defined here?
		for(auto &b : thirds) bonds.push_back(bond{b[0],b[1],physical,-1,3});  // z seems ill-defined here?
		for(auto &b : fourths_plus) rings.push_back(ring{b[0],b[1],b[2],b[3],physical,-1, 4, 1});  // z seems ill-defined here?
		for(auto &b : fourths_sub) rings.push_back(ring{b[0],b[1],b[2],b[3],physical,-1, 4, -1});  // z seems ill-defined here?
		/* sites */
		for(int i : range1(N)) sites.push_back(site{i,positions[i-1][0],positions[i-1][1],positions[i-1][2],physical});
	}

	void print_lattice()
    {
    	
    	cout << "bonds = " << endl << "[";
    	for(auto &b : bonds) cout << "[" << b.s1 << "," << b.s2 << "]" << endl;
    	cout << "]";
    	cout << "adjmat = " << endl;  print(adjmat);
    	cout << "seconds = " << endl; print(seconds);
    	cout << "thirds = " << endl;  print(thirds);
    }

    void save_lattice(Repository* repo)
    {
    	auto labels = vector<string>({"x","y"});
    	auto labels_tri = vector<string>({"x","y","z"});
    	auto labels_fourth = vector<string>({"x1","y1","x2","y2"});
    	auto labels_adjmat = vector<string>();
    	auto Nx = args->getInt("Nx");
		auto Ny = args->getInt("Ny");
		auto N = Nx*Ny;
    	for(auto i : range(N)) labels_adjmat.push_back(to_string(i));
    	vector<vector<int>> vec_bonds;
    	for(auto &b : bonds)
    	{
    		if (b.n == 1)
    			vec_bonds.push_back(vector<int>{b.s1,b.s2});   
    	}
    	vector<vector<Real>> vec_sites;
    	for(auto &s : sites)
    	{
    		if (s.t == physical)
    			vec_sites.push_back(vector<Real>{s.rx,s.ry});   
    	}


    	repo->save("triangles","temp",labels_tri,triangles,true);
    	repo->save("fourths_plus","temp",labels_fourth,fourths_plus,true);
    	repo->save("fourths_sub","temp",labels_fourth,fourths_sub,true);
    	repo->save("sites","temp",labels,vec_sites,true);
    	repo->save("bonds","temp",labels,vec_bonds,true);
    	repo->save("seconds","temp",labels,seconds,true);
    	repo->save("thirds","temp",labels,thirds,true);
    	repo->save("adjmat","temp",labels_adjmat,adjmat,true);

    	// repo->save(Correlation::getHash(args),"correlation"+type+"/"+args->getString("Model"),labels,results); //<--- Update
    }


protected:

	void get_lattice(int Nx, int Ny)
	{
		auto geometry = args->getString("Geometry");
		if(geometry == "YC")
		{
			lat = triangularLattice(Nx,Ny,*args);
			get_positions_YC(Nx,Ny);
		}
		else
		if(geometry == "XC")
		{
			get_lattice_XC(Nx,Ny);
		}
		else
			Error("Geometry specified is not implemented: "+geometry);
	}

	void get_positions_YC(int Nx, int Ny)
	{
		int N = Nx*Ny;
		Real ax = sqrt(3.0)/2.0;
	    Real ay = 1.0;
	    Real shift = -0.5;
		for(int n = 1; n <= N; ++n)
        {
	        int x = (n-1)/Ny+1; 
	        int y = (n-1)%Ny+1;
	        auto rx = ax*x;
	        auto ry = ay*y + x*shift;
	        while(ry < 0)
	        	ry += ay*Ny;
	        positions.push_back({rx,ry,0.0});
    	}

	}

	void get_lattice_XC(int Nx, int Ny)
	{
		lat.clear();
        auto PBC = args->getBool("YPeriodic");
        int N = Nx*Ny;
        Real ax = 1.0;
        Real ay = sqrt(3.0)/2.0;
        Real shift = 0.5;
        for(int n = 1; n <= N; ++n)
        {
        	int x = (n-1)/Ny; 
	        int y = (n-1)%Ny;
	        auto rx = ax*x;
	        auto ry = ay*y;
	        if((y%2) == 1)
	        	rx += shift;	        
	        positions.push_back({rx,ry,0.0});
            //up
	        int n2 = n+1;
	        if (n2 <= N)
	        	lat.emplace_back(n,n2);
	        n2 = n+Ny;
	        if (n2 <= N)
	        	lat.emplace_back(n,n2);
	        if (((n%2) == 1) & (y != 0))
	        	continue;
	       	n2 = n+Ny-1;
	       	if (n2 <= N)
	        	lat.emplace_back(n,n2);
	        if (((n%2) == 0) & ((n%Ny) != 0))
	        {
	        	n2 = n+Ny+1;
	        	if (n2 <= N)
		        	lat.emplace_back(n,n2);
	        }
        }
	}

    void get_adjmat(int N)
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
    
    void get_seconds(int Nx, int Ny) // adjmat used
    {
    	int N = Nx*Ny;
    	seconds = vector<vector<int>>();
    	for(int n = 1; n <= N; ++n)
    	{
    		int x = (n-1)/Ny; 
	        int y = (n-1)%Ny;
	        auto r2os = vector<vector<int>>{vector<int>{x, y+2},
	        							   vector<int>{x+1, y+1}};
	        auto r2es = vector<vector<int>>{vector<int>{x, y+2},
	        							   vector<int>{x+2, y+1}};
	        if(Ny > 2)
	        {
	        	r2os.push_back(vector<int>{x+1, y-1});
	        	r2es.push_back(vector<int>{x+2, y-1});
	        }
	        auto r2s = vector<vector<int>>();
	        if (n%2 == 0) r2s = r2es;
	        else r2s = r2os;
	        if(Ny > 4) r2s.push_back(vector<int>{x, y-2});
	        for(auto r2 : r2s)
	        {
	        	auto x2 = r2[0];
	        	auto y2 = r2[1];
	        	if (x2 >= Nx) continue;
	        	if (y2 < 0) y2 += Ny;
	        	if (y2 >= Ny) y2 -= Ny;
	        	auto n2 = x2*Ny + y2 + 1;
	        	if (n2 > n) seconds.push_back(vector<int>{n, n2});
	        }
	        // if(y == 1)
	        // {
	        // 	int n2 = n+2;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+2*Ny+1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+2*Ny-1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+Ny-2;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});

	        // }
	        // else
	        // if(y == 0)
	        // {
	        // 	int n2 = n+2;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+Ny+1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+2*Ny-1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+Ny-2;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // }
	        // else
	        // if(y == (Ny-1))
	        // {
	        // 	int n2 = n+Ny+1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+2*Ny-1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // }
	        // else
	        // if(y == (Ny-2))
	        // {
	        // 	int n2 = n+Ny+1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // 	n2 = n+Ny-1;
	        // 	if(n2 <= N)
	        // 		seconds.push_back(vector<int>{n,n2});
	        // }
	        // else
	        // {
	        // 	if((n%2) == 0)
	        // 	{
	        // 		int n2 = n+2;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
		       //  	n2 = n+2*Ny+1;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
		       //  	n2 = n+2*Ny-1;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
	        // 	}
	        // 	if((n%2) == 1)
	        // 	{
	        // 		int n2 = n+2;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
		       //  	n2 = n+Ny+1;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
		       //  	n2 = n+Ny-1;
		       //  	if(n2 <= N)
		       //  		seconds.push_back(vector<int>{n,n2});
	        // 	}
	        // }
    	}
	    // for(auto _tri : triangles)
	    // {
	    //     set<int> tri; for(auto x : _tri) tri.insert(x);
	    //     for(auto _tri2 : triangles)
	    //     {
	    //         set<int> tri2; for(auto x : _tri2) tri2.insert(x);
	    //         set<int> tri_intersect;
	    //         set_intersection(tri.begin(), tri.end(), tri2.begin(), tri2.end(), std::inserter(tri_intersect, tri_intersect.begin()));
	    //         if(tri_intersect.size() == 2) // 2nd neighbor found
	    //         {
	    //             set<int> tri_union;
	    //             set_union(tri.begin(), tri.end(), tri2.begin(), tri2.end(), std::inserter(tri_union, tri_union.begin()));
	    //             set<int> _second;
	    //             set_difference(tri_union.begin(), tri_union.end(), tri_intersect.begin(), tri_intersect.end(), std::inserter(_second, _second.begin()));
	    //             vector<int> second (_second.begin(),_second.end());
	    //             adjmat[second[0]][second[1]] = 2;
	    //         }
	    //     }
	    // }

	    // seconds = vector<vector<int>>();
	    // for(int i : range(adjmat.size()))
	    // {
	    // 	for(int j : range(i+1,adjmat[i].size()))
	    // 	{
	    // 		if (adjmat[i][j] == 2)
	    // 		{
	    // 			seconds.push_back(vector<int>{i,j});
	    // 		}
	    // 	}
	    // }
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

    void get_fourths() // adjmat used
    {

    	vector<vector<int>> rings;
    	for(auto _tri1 : triangles)
    	{
    		set<int> tri1; for(auto x : _tri1) tri1.insert(x);
    		for(auto _tri2 : triangles)
	    	{
	    		set<int> tri2; for(auto x : _tri2) tri2.insert(x);
	    		set<int> tri_intersect;
	    		set_intersection(tri1.begin(), tri1.end(), tri2.begin(), tri2.end(), std::inserter(tri_intersect, tri_intersect.begin()));
	    		if(tri_intersect.size() == 2) // 2nd neighbor found
	            {
	                set<int> tri_union;
	                set_union(tri1.begin(), tri1.end(), tri2.begin(), tri2.end(), std::inserter(tri_union, tri_union.begin()));
	                vector<int> ring(tri_union.begin(),tri_union.end());
	                rings.push_back(ring);
	            }
	    	}
    	}

    	for (auto ring : rings)
    	{
    		auto s1 = ring[0];
    		for(auto j : range(1,ring.size()))
    		{
    			auto s2 = ring[j];
    			auto pair = vector<int>{s1,s2};
    			sort(pair.begin(),pair.end());
    			auto off = vector<int>(ring);
    			off.erase(std::remove(off.begin(), off.end(), pair[0]), off.end());
    			off.erase(std::remove(off.begin(), off.end(), pair[1]), off.end());
    			sort(off.begin(),off.end());
    			auto res = vector<int>{pair[0],pair[1],off[0],off[1]};
    			if( (adjmat[pair[0]][pair[1]] == 2) || (adjmat[off[0]][off[1]] == 2) ) // Subtract these
    			{
    				fourths_sub.push_back(res);
    			}
    			else
    			{
    				fourths_plus.push_back(res);
    			}
    		}
    	}

    }

    

    
};

#endif