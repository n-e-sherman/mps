// #ifndef __THERMALTRIANGULAR_H_
// #define __THERMALTRIANGULAR_H_

// #include "lattice/lattice.h"
// #include "lattice/triangular.h"
// #include "repository/repository.h"

// using namespace std;
// using namespace itensor;

// class ThermalTriangular : public Triangular
// {
// protected:


// public:
	

// 	ThermalTriangular(Args* a) : args(a) 
// 	{	
// 		auto Nx = args->getInt("Nx");
// 		auto Ny = args->getInt("Ny");
// 		auto N = Nx*Ny;
// 		if(N != args->getInt("N")) { cout << "N is not equal to Nx*Ny, Modifying this." << endl; args->add("N",N); }

// 		/* lattice data */
// 		get_lattice(Nx,Ny);
// 		get_adjmat(N);
// 		get_triangles();
// 		get_seconds();
// 		get_thirds();
// 		get_fourths();


// 		/* bonds */
// 		for(auto &l : lat) 
// 		{
// 			bonds.push_back(bond{2*l.s1-1,2*l.s2-1,physical,-1,1});  // z seems ill-defined here?
// 			bonds.push_back(bond{2*l.s1,2*l.s2,environment,-1,1});  // z seems ill-defined here?
// 		}
// 		for(auto &b : seconds) 
// 		{
// 			bonds.push_back(bond{2*b[0]-1,2*b[1]-1,physical,-1,2});  // z seems ill-defined here?
// 			bonds.push_back(bond{2*b[0],2*b[1],environment,-1,2});  // z seems ill-defined here?
// 		}
// 		for(auto &b : thirds) 
// 		{
// 			bonds.push_back(bond{2*b[0]-1,2*b[1]-1,physical,-1,3});  // z seems ill-defined here?
// 			bonds.push_back(bond{2*b[0],2*b[1],environment,-1,3});  // z seems ill-defined here?
// 		}
// 		for(auto &b : fourths_plus) 
// 		{
// 			rings.push_back(ring{2*b[0]-1,2*b[1]-1,2*b[2]-1,2*b[3]-1,physical,-1, 4, 1});  // z seems ill-defined here?
// 			rings.push_back(ring{2*b[0],2*b[1],2*b[2],2*b[3],environment,-1, 4, 1});  // z seems ill-defined here?
// 		}
// 		for(auto &b : fourths_sub) 
// 		{
// 			rings.push_back(ring{2*b[0]-1,2*b[1]-1,2*b[2]-1,2*b[3]-1,physical,-1, 4, -1});  // z seems ill-defined here?
// 			rings.push_back(ring{2*b[0],2*b[1],2*b[2],2*b[3],environment,-1, 4, -1});  // z seems ill-defined here?
// 		}
// 		/* sites */
// 		for(int i : range1(N)) 
// 		{
// 			sites.push_back(site{2*i-1,positions[i-1][0],positions[i-1][1],positions[i-1][2],physical});
// 			sites.push_back(site{2*i,positions[i-1][0],positions[i-1][1],positions[i-1][2],environment});
// 		}
// 	}
    
// };

// #endif