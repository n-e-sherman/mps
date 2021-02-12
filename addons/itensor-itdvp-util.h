#ifndef __ITENSOR_ITDVP_UTIL_H
#define __ITENSOR_ITDVP_UTIL_H

#include "itensor/all.h"
#include <vector>

using namespace itensor; 
using namespace std;

/* Globals */
// extern complex<double> _i; 

ITensor subTensor(ITensor T, vector<Index> indices, vector<int> values)
{
	ITensor retT;
	for(unsigned l=0; l<indices.size();l++)
	{
		if(l==0)
	  		retT = T*setElt(indices[l]=values[l]);
		else
	  		retT*=setElt(indices[l]=values[l]);
	}
	return retT;
}

vector<vector<ITensor> > extract_Tw(ITensor& A, ITensor& W, Index a, Index b) 
{
	Index is = findIndex(A, "Site"); 
	int dw = dim(a); 
	vector<vector<ITensor> > Tw(dw, vector<ITensor>(dw));  
	for(int i = 0; i < dw; i++)
	{
		for(int j = 0; j < dw; j++)
		{
			ITensor Wab = subTensor(W, {a, b}, {i+1, j+1}); 
			Tw[i][j] = Wab * A * prime(dag(A)); 
		}
	}
	return Tw; 
}

void print_MPO(ITensor W) 
{
	auto ia = findInds(W, "Link")(1);  
	auto ib = findInds(W, "Link")(2);  
	auto is = findInds(W, "Site")(1); 
	PrintData(ia);
	PrintData(ib); 
	PrintData(is); 
	cout << "The MPO tensor W is: " << endl; 
	int width = 12; 
	for(int i = 1; i <= dim(ia); i++)
	{
		for(int s = 1; s <= dim(is); s++)
		{
			for(int j = 1; j <= dim(ib); j++)
			{
				ITensor Wab = subTensor(W, {ia, ib}, {i, j}); 
				for(int ss = 1; ss <= dim(is); ss++)
				{
					cout.width(width); 
					std::ostringstream out;  
					out.precision(2); 
					if(abs(imag((eltC(Wab, is(s), prime(is)(ss))))) < 1E-5)
					{
						double num = real(eltC(Wab, is(s), prime(is)(ss))); 
						if(abs(num-int(num)) < 1E-5)
						{
							out.precision(0); 
							out << std::fixed << num; 
							out.precision(2); 
						}
						else 
						{
							out << std::fixed << num; 
						}
					}
					else 
					{
						Cplx num = (eltC(Wab, is(s), prime(is)(ss))); 
						out << std::fixed << real(num) << std::showpos << imag(num) << "i"; 
					}

					string str; 
					if(ss == 1)
						str = "|" + out.str(); 
					else 
						str = out.str(); 

					cout << left << str;   
				}
			}
	  	cout << endl; 
		}
		for(int k = 1; k <= dim(ib)*dim(is)*width; k++)
		{
			cout << "-"; 
		}
		cout << endl; 
	}
}

class BigM_L 
{
public: 
    ITensor E_; 
     
    Index il_; 
    Index ir_; 
    ITensor r_;
    ITensor Il_; 
    double lambda_; 
    bool isR_; 
    

    BigM_L(ITensor E, Index i_l, Index i_r, ITensor r, double lambda, bool isR)
              :E_(E),  il_(i_l),  ir_(i_r),     r_(r),lambda_(lambda),isR_(isR)
    {
		Il_ = ITensor(il_, prime(il_)); 
		for(int i = 1; i <= dim(il_); i++)
		{
			Il_.set(il_(i), prime(il_)(i), 1); 
		}
    }

    int size() const
    {
      return dim(il_) * dim(il_); 
    }

    void product(ITensor& x, ITensor& Ax) const
    {
		ITensor L1, L2, L3; 
		L1 = x;  
		L2 = (x*lambda_*E_) * delta(il_, ir_) * delta(prime(il_), prime(ir_)); 
		if(!isR_)
		{
			Ax = L1 - L2; 
		}
		else
		{
			L3 = x * delta(il_, ir_) * delta(prime(il_), prime(ir_)) * r_ * Il_; 
			Ax = L1 - L2 + L3; 
		} 
    }
};

class BigM_R 
{
public: 
	ITensor E_; 
	
	ITensor Ir_; 
	Index il_; 
	Index ir_; 
	ITensor l_; 
	double lambda_; 
	bool isL_; 
	

    BigM_R(ITensor E, Index i_l, Index i_r, ITensor l, double lambda, bool isL)
              :E_(E),  il_(i_l),  ir_(i_r),     l_(l),lambda_(lambda),isL_(isL)
    {
		Ir_ = ITensor(ir_, prime(ir_)); 
		for(int i = 1; i <= dim(ir_); i++)
		{
			Ir_.set(ir_(i), prime(ir_)(i), 1); 
		}
    }

    int size() const
    {
      return dim(ir_) * dim(ir_); 
    }

    void product(ITensor& x, ITensor& Ax) const
    {
		ITensor L1, L2, L3; 
		L1 = x;  
		L2 = (lambda_*E_*x) * delta(il_, ir_) * delta(prime(il_), prime(ir_)); 
		if(!isL_)
		{
			Ax = L1 - L2; 
		}
		else 
		{
			L3 = l_ * delta(il_, ir_) * delta(prime(il_), prime(ir_)) * x * Ir_; 
			Ax = L1 - L2 + L3; 
		}
    }
};

class K_op
{
public: 
	ITensor lw_; 
	ITensor rw_; 
	Index il_, ir_; 

    K_op(ITensor lw, ITensor rw, Index il, Index ir) : lw_(lw),    rw_(rw),  il_(il),   ir_(ir) {}
   
    int size() const
    {
		return dim(il_) * dim(ir_); 
    }

    void product(ITensor& C, ITensor& K_C) const
    {
		K_C = C * lw_ * rw_; 
		K_C.noPrime(); 
		return; 
    }
};

class BigMatrix 
{
public: 

	ITensor E_; 
	Index i_open_; 
	Index i_contact_; 
	

    BigMatrix(ITensor& E, Index i_open, Index i_contact) : E_(E), i_open_(i_open), i_contact_(i_contact) {}

    int size() const
    {
		return dim(i_open_); 
    }

    void product(ITensor& x, ITensor& Ex) const //i_contact-x
    {
		ITensor x_tmp = x * delta(i_contact_, prime(i_contact_)); 
		Ex = (E_ * delta(i_contact_, prime(i_contact_))) * x_tmp; //iopen-Ex 
		Ex = Ex * delta(i_open_, i_contact_); 
		return; 
    }
};

#endif
