#ifndef __ITENSOR_ITDVP_H
#define __ITENSOR_ITDVP_H

#include"itensor/all.h"
#include "addons/itensor-itdvp-util.h"
#include<vector>
#include<iostream>
#include<random>
# include<iomanip>
# include<string>

using namespace itensor;
using namespace std; 

//The code is based on PHYSICAL REVIEW B 97, 045145 (2018)
//and SciPost Phys. Lect. Notes 7 (2019), and follows their
//notation. One should cite these papers when using this code. 


/* shared preamble of imps.cc and imps.h */
// # include<iostream>
// # include<vector>
// #include "util.h"
// # include "imps.h"
// using namespace std; 




/* Globals */
// typedef ITensor T; 
 


ITensor identity(Index a, Index b)
{
	ITensor id(a, b); 
	for(int i = 1; i <= dim(a); i++) { id.set(a(i), b(i), 1); }
	return id; 
}
/*------------------------------------------------------------*/
class iMPS
{
protected:
	double gmres_err = 10E-15, gauge_err = 10E-15;
	int m_; 
    ITensor A_, AL_, AR_, C_, AC_, lw_, rw_, L_, R_;   
    ITensor XR_, XL_; //guess vectors when computing lw and rw
    Index il_, ir_; 
    Index is_;
    ITensor W_; 
    Index wl_, wr_; 
    double entropy_, energy_; 
    ofstream out_fs_; 

public:

	iMPS(){}
    iMPS(ITensor A, Index il, Index ir, Index is, ITensor W, Index wl, Index wr, string s) :m_(dim(il)), 
     		 A_(A),  il_(il),  ir_(ir),  is_(is),     W_(W),  wl_(wl),  wr_(wr)
	{
		A_ = ITensor(il_, is_, ir_); 
		A_.set(1, 1, 1, 1.0);      
		XR_ = randomITensor(ir_, prime(ir_)); 
		XL_ = randomITensor(il_, prime(il_)); //to be used in gmres 
		set_canonical(); 
		lw_ = get_lw(); 
		rw_ = get_rw(); 
		get_energy(false); 
		get_entropy(false); 

		out_fs_.open("imps_"+s+".out"); 
	}

    void init_A();

    void set_canonical()
	{
		mixed_gauge(); //il-AL-il', il'-C_-ir', ir'-AR-ir
		AC_ = AL_ * delta(ir_, prime(ir_)) * (C_ * delta(il_, prime(ir_))); // il-AC-ir 
	}

    void set_MPO(ITensor W, Index wl, Index wr)
	{
		W_ = W; 
		wl_ = wl; 
		wr_ = wr; 
		lw_ = get_lw(); 
		rw_ = get_rw(); 
	}

    void mixed_gauge()
	{
		//Algorithm 2 in SciPost Phys. Lect. Notes 7 (2019)
  		//input: il-A-ir; 
  		//return: il-AL-ir, il-C-ir, il-AR-ir, il'-L-il, ir-R-ir'

		L_ = identity(il_, prime(il_)); 
		AL_ =  left_normal(gauge_err);  
		R_ = identity(ir_, prime(ir_)); 
		AR_ = right_normal(gauge_err); 

		C_ = L_ * R_ * delta(il_, ir_);  
		C_ = C_ * delta(il_, prime(il_)) * delta(ir_, prime(ir_)); 
		C_ = C_/norm(C_); 
	}

    void iTDVP(Cplx dt) 
	{
		//"Integrating the TDVP equations" on page 35 of SciPost Phys. Lect. Notes 7 (2019)
		//input: il, ir, il-AC-ir, il-C-ir, il-AL-ir, il-AR-ir
		//output: il-AC'-ir, il-C'-ir, il-AL'-ir, il-AR'-ir
		LocalOp Heff(W_, lw_, rw_, {"numCenter=",1}); // <---- Make a parameter? Maybe this is forced?
		applyExp(Heff,AC_,dt);

		lw_ *= delta(wl_, wr_); 
		K_op Keff(lw_, rw_, il_, ir_); //K_op is defined in util.h
		applyExp(Keff,C_,dt); 

		AL_ = split_AL(); //il_-AL-ir_
		AR_ = split_AR(); //il_-AR-ir_ 
		C_ /= norm(C_); 
		AC_ /= norm(AC_); 

		lw_ = get_lw(); 
		rw_ = get_rw(); 
	}

    double get_energy(bool is_print) 
	{
		ITensor Ew = W_ * AL_ * prime(dag(AL_), il_, ir_, is_); 
		ITensor lw_1 = lw_ * Ew * delta(il_, ir_) * delta(prime(il_), prime(ir_)) * delta(wl_, wr_); 

		Ew = W_ * AR_ * prime(dag(AR_), il_, ir_, is_); 
		ITensor rw_1 = rw_ * Ew * delta(il_, ir_) * delta(prime(il_), prime(ir_)) * delta(wl_, wr_); 
		if(is_print)
		{
		PrintData(lw_1 - lw_); 
		PrintData(rw_1 - rw_); 
		}
		int dw = dim(findInds(W_, "Link")(1)); 
		double energy_l = real(eltC(lw_1 - lw_, il_(1), prime(il_)(1), wl_(1)));  
		double energy_r = real(eltC(rw_1 - rw_, ir_(1), prime(ir_)(1), wr_(dw)));  

		energy_ = energy_l; 
		return energy_l; 
	}

    double get_entropy(bool is_print)
	{
		ITensor U(il_),S,V;        
		auto spec = svd(C_, U, S, V); 
		if(is_print)
		{
			cout << "itdvp spectrum: " << endl; 
			PrintData(S); 
		}

		auto eigs = spec.eigsKept(); 
		double entropy = 0.0; 
		for(auto& p: eigs) { if(p > 1E-13) { entropy += -p * log(p); } }
		entropy_ = entropy; 

		return entropy; 
	}

    double check_canonical(bool is_print) 
	{
		Index ii(dim(il_)); 
		ITensor AL_C = AL_ * delta(ir_, ii) * (C_ * delta(il_, ii)); 
		ITensor C_AR = C_ * delta(ir_, ii) * (AR_ * delta(il_, ii)); 
		if(is_print)
		{
			cout << "=============check_canonical===========" << endl; 
			PrintData(AL_ * prime(dag(AL_), ir_));   
			PrintData(AR_ * prime(dag(AR_), il_));   
			PrintData(AC_ - AL_C);  
			PrintData(AC_ - C_AR); 
			cout << "=============done checking  ===========" << endl; 
		}
		return norm(AC_ - AL_C); 
	}

    ITensor left_normal(double err_goal) 
	{
		//Algorithm 1 in SciPost Phys. Lect. Notes 7 (2019)
		//input: il_'-L_guess-il_, il_-A-ir_  
		//return: il_-AL-ir_, il_'-L-il_
		L_ /= norm(L_); 
		ITensor L_old = L_; 

		ITensor L_A = (L_ * delta(il_, prime(ir_))) * (A_ * delta(il_, prime(ir_))); //il_'-L_A-ir_ 
		ITensor AL = ITensor(is_, prime(il_)); 

		auto [AL_temp, L_temp] = qr(L_A, AL.inds()); 
		AL = AL_temp;
		L_ = L_temp; 

		Index iQR = findIndex(L_, "QR"); 
		AL = AL * delta(prime(il_), il_) * delta(iQR, ir_); 
		L_ = L_ * delta(iQR, prime(il_)) * delta(ir_, il_); //il_'-L-il_ 
		L_ /= norm(L_);
		Real err = norm(L_-L_old); 
		int i = 1;  
		while(err > err_goal)
		{
			ITensor E = A_ * prime(dag(AL), il_, ir_);  

			auto [CC, ic] = combiner(prime(il_), il_);//contact 
			auto [O, io] = combiner(prime(ir_), ir_);//open 
			auto E_comb = E * CC * O; 

			auto E_big = BigMatrix(E_comb, io, ic);  
			ITensor guess = L_ * CC; //guess-ic 
			arnoldi(E_big, guess, {"ErrGoal=", err/10.});  
			L_ = guess * CC; 
			L_ /= norm(L_); 
			L_old = L_; 

			L_A = (L_ * delta(il_, prime(ir_))) * (A_ * delta(il_, prime(ir_))); //il_'-L_A-ir_ 
			AL = ITensor(is_, prime(il_)); 


			auto [AL_temp, L_temp] = qr(L_A, AL.inds()); 
			AL = AL_temp;
			L_ = L_temp; 

			iQR = findIndex(L_, "QR"); 
			AL = AL * delta(prime(il_), il_) * delta(iQR, ir_); 
			L_ = L_ * delta(iQR, prime(il_)) * delta(ir_, il_); //il_'-L-il_ 
			L_ /= norm(L_);
			err = norm(L_-L_old); 
			i++; 
		}

		return AL; 
	}

    ITensor right_normal(double err_goal)	
	{
		//right version of Algorithm 1 in SciPost Phys. Lect. Notes 7 (2019)
		//input: il_-A-ir_, ir_-R_guess-ir_' 
		//return: il_-AR-ir_, ir_-R-ir_'
		R_ /= norm(R_); 
		ITensor R_old = R_; 

		ITensor A_R = (A_ * delta(ir_, prime(il_))) * (R_ * delta(prime(il_), ir_)); //il_-A_R-ir_' 
		ITensor AR = ITensor(is_, prime(ir_)); 

		auto [AR_temp, R_temp] = qr(A_R, AR.inds()); 
		AR = AR_temp;
		R_ = R_temp; 

		Index iQR = findIndex(R_, "QR"); 
		AR = AR * delta(prime(ir_), ir_) * delta(iQR, il_); 
		R_ = R_ * delta(iQR, prime(ir_)) * delta(il_, ir_); //ir_-R-ir_' 
		R_ /= norm(R_);
		Real err = norm(R_-R_old); 
		while(err > err_goal)
 		{
		    ITensor E = A_ * prime(dag(AR), ir_, il_);  

		    auto [CC, ic] = combiner(prime(ir_), ir_);//contact 
		    auto [O, io] = combiner(prime(il_), il_);//open 
		    auto E_comb = E * CC * O; 

		    auto E_big = BigMatrix(E_comb, io, ic);  
		    ITensor guess = R_ * CC; //guess-ic 
		    arnoldi(E_big, guess, {"ErrGoal=", err/10.});  
		    R_ = guess * CC; 
		    R_ /= norm(R_); 
		    R_old = R_; 

		    A_R = (A_ * delta(ir_, prime(il_))) * (R_ * delta(prime(il_), ir_)); //il_-A_R-ir_' 
		    AR = ITensor(is_, prime(ir_)); 
		    auto [AR_temp, R_temp] = qr(A_R, AR.inds()); 
		    AR = AR_temp;
		    R_ = R_temp; 

		    iQR = findIndex(R_, "QR"); 
		    AR = AR * delta(prime(ir_), ir_) * delta(iQR, il_); 
		    R_ = R_ * delta(iQR, prime(ir_)) * delta(il_, ir_); //ir_-R-ir_' 
		    R_ /= norm(R_);
		    err = norm(R_-R_old); 
  		}
		return AR; 
	}

    ITensor get_l()
	{
		//return the leading left eigenvector of AR_*dag(AR_) 
		ITensor r = identity(ir_, prime(ir_)); 
		auto l_AR = C_ * delta(il_, prime(il_)) * (dag(C_) * delta(il_, prime(il_)) * delta(ir_, prime(ir_)));//ir_-l-ir_' 
		l_AR = l_AR * delta(ir_, il_) * delta(prime(ir_), prime(il_)); 
		Cplx tr = eltC((r * l_AR * delta(ir_, il_)) * delta(prime(il_), prime(ir_)));  
		return l_AR/tr; 
	}

    ITensor get_r() 
	{
		//return the leading right eigenvector of AL_*dag(AL_) 
		ITensor l = identity(il_, prime(il_)); 
		auto r_AL = C_ * delta(ir_, prime(ir_)) * (dag(C_) * delta(ir_, prime(ir_)) * delta(il_, prime(il_))); //il_-r-il_' 
		r_AL = r_AL * delta(il_, ir_) * delta(prime(il_), prime(ir_)); 
		Cplx tr = eltC((l * r_AL * delta(ir_, il_)) * delta(prime(il_), prime(ir_)));  
		return r_AL/tr; 
	}

    ITensor get_lw()
	{
		//Algorithm 6 in PRB 97, 045145 (2018)
		//return the left leading generalized eigenvector of EW_AL
		ITensor E = AL_ * prime(dag(AL_), il_, ir_);  
		Index is_ = commonIndex(W_, AL_); 
		ITensor l = ITensor(il_, prime(il_));

		for(int i = 1; i <= dim(il_); i++) { l.set(il_(i), prime(il_)(i), 1); }
		ITensor r_AL = get_r(); 

		int dw = dim(wl_); 
		vector<vector<ITensor> > Tw = extract_Tw(AL_, W_, wl_, wr_);   
		vector<ITensor> Lw(dw, ITensor(il_, prime(il_)));  
		for(int a = dw; a > 0; a--)
		{
			if(a == dw) 
			{
		  		Lw[a-1] = l; 
			}
			else
			{
				ITensor Ya(ir_, prime(ir_)); 
				for(int b = dw; b > a; b--) { Ya += Lw[b-1] * Tw[b-1][a-1]; }
				Ya *= delta(il_, ir_);
				Ya *= delta(prime(il_), prime(ir_)); 

		  		if(a > 1) //solve (L1|(1- lambda*E) =  (Ya| 
		  		{ 
				    double lambda = 0; //lambda = 0 for nn-TFIM
				    ITensor guess = Ya; 
				    BigM_L M(E, il_, ir_, r_AL, lambda, false); 
					gmres(M, Ya, guess, {"ErrGoal=", gmres_err});	
				    Lw[a-1] = guess;
		  		}
				else if(a == 1) //solve (L1|(1- E +|R)(1|) =  (Ya| - (Ya|R)(1|
				{ 
					ITensor Y1 = Ya; 
					ITensor Y2 = (Ya * r_AL * delta(ir_, il_)) * delta(prime(il_), prime(ir_)) * l;  
					ITensor Y = Y1-Y2; 
					BigM_L M(E, il_, ir_, r_AL, 1.0, true); 
					gmres(M, Y, XL_, {"ErrGoal=", gmres_err});	

					Lw[a-1] = XL_;   
				}
				else 
				{
					cerr << "wrong" << endl; 
				}
			}
		}

		ITensor lw(il_, wl_, prime(il_)); 
		for(int i = 1; i <= dim(il_); i++)
		{
			for(int j = 1; j <= dim(prime(il_)); j++)
			{
				for(int a = 1; a <= dim(wl_); a++)
				{
					lw.set(il_(i), prime(il_)(j), wl_(a), eltC(Lw[a-1], il_(i), prime(il_)(j))); 
				}
			}
		}
		return lw; 
	}

    ITensor get_rw() 
	{
		//return the right leading generalized eigenvector of EW_AL
		ITensor E = AR_ * prime(dag(AR_), il_, ir_);  
		ITensor r = ITensor(ir_, prime(ir_)); 
		for(int i = 1; i <= dim(ir_); i++) { r.set(ir_(i), prime(ir_)(i), 1); }
		ITensor l_AR = get_l(); 

		int dw = dim(wr_); 
		vector<vector<ITensor> > Tw = extract_Tw(AR_, W_, wl_, wr_);  
		vector<ITensor> Rw(dw, ITensor(ir_, prime(ir_)));  
		for(int a = 1; a <= dw; a++) 
		{
			if(a == 1) { Rw[a-1] = r; }
			else
			{
				ITensor Ya(il_, prime(il_)); 
				for(int b = 1; b < a; b++) { Ya += Tw[a-1][b-1] * Rw[b-1]; }
				Ya *= delta(il_, ir_);
				Ya *= delta(prime(il_), prime(ir_)); 

				if(a < dw)
				{
					double lambda = 0; //lambda = 0 for nn-TFIM

					ITensor guess = Ya; 
					BigM_R M(E, il_, ir_, l_AR, lambda, false); 
					gmres(M, Ya, guess, {"ErrGoal=", gmres_err});	
					Rw[a-1] = guess;  
				}
				else if(a == dw) //solve (L1|(1- TL_ +|R)(1|) =  (Ya| - (Ya|R)(1|
				{
					ITensor Y1 = Ya; 
					ITensor Y2 = (Ya * l_AR * delta(ir_, il_)) * delta(prime(il_), prime(ir_)) * r;  
					ITensor Y = Y1-Y2; 

					BigM_R M(E, il_, ir_, l_AR, 1.0, true); 
					gmres(M, Y, XR_, {"ErrGoal=", gmres_err});	
					Rw[a-1] = XR_;   
				}
				else 
				{
					cerr << "wrong" << endl; 
				}
			}
		}
		ITensor rw(ir_, wr_, prime(ir_)); 
		for(int i = 1; i <= dim(ir_); i++)
		{
			for(int j = 1; j <= dim(prime(ir_)); j++)
			{
		  		for(int a = 1; a <= dim(wr_); a++)
		  		{
		    		rw.set(ir_(i), prime(ir_)(j), wr_(a), eltC(Rw[a-1], ir_(i), prime(ir_)(j))); 
		  		}
			}
		}
		return rw; 
	}

    ITensor split_AL() 
	{
		//Eq. 139-142 in SciPost Phys. Lect. Notes 7 (2019) 
		//input: il_-AC_-ir_, il_-C_-ir_; 
		//return: il_-AL-ir_ 
		ITensor AC_Cdag = AC_ * dag(C_ * delta(il_, prime(il_))); 
		Index is = findIndex(AC_Cdag, "Site"); 
		ITensor U(il_, is), S, V; 
		svd(AC_Cdag, U, S, V); //AC_Cdag = U*S*V 
		Index iU = commonIndex(U, S); 
		Index iV = commonIndex(S, V); 
		ITensor AL = U * V * delta(iU, iV); 
		AL *= delta(prime(il_), ir_); 
		return AL; 
	}

    ITensor split_AR()   
	{
		//Eq. 139-142 in SciPost Phys. Lect. Notes 7 (2019) 
		//input: il_-AC_-ir_, il_-C_-ir_; 
		//return: il_-AR-ir_ 
		ITensor Cdag_AC = dag(C_ * delta(ir_, prime(ir_))) * AC_;  
		ITensor U(prime(ir_)), S, V;  
		svd(Cdag_AC, U, S, V); 
		Index iU = commonIndex(U, S); 
		Index iV = commonIndex(S, V); 
		ITensor AR = U * V * delta(iU, iV);
		AR *= delta(prime(ir_), il_); 
		return AR; 
	}

    void to_screen(int i, double t)
	{
		double entropy = get_entropy(true); 
		double energy = get_energy(false); 
		cout << setprecision(16); 
		cout << "itdvp entropy: " << entropy << endl; 
		cout << "itdvp energy_l: " << energy << endl;
	}

    void to_file(int i, double t)
	{
		ITensor Sx = ITensor(is_, prime(is_));  
		Sx.set(is_(1), prime(is_)(2), 1); 
		Sx.set(is_(2), prime(is_)(1), 1); 

		Cplx mx = eltC(delta(il_, prime(il_)) * AC_ * prime(dag(AC_)) * Sx * delta(ir_, prime(ir_)));  

		entropy_ = get_entropy(false);  
		energy_ = get_energy(false); 
		out_fs_ << setprecision(16); 
		out_fs_ << i << "\t" << abs(t) << "\t" << entropy_ << "\t" << energy_ << "\t" << real(mx) << endl; 
	}

	void read(istream& is) // Poorly implemented...
    {
    	itensor::read(is,m_);
    	itensor::read(is,A_);
    	itensor::read(is,AL_);
    	itensor::read(is,AR_);
    	itensor::read(is,C_);
    	itensor::read(is,AC_);
    	itensor::read(is,lw_);
    	itensor::read(is,rw_);
    	itensor::read(is,L_);
    	itensor::read(is,R_);
    	itensor::read(is,XL_);
    	itensor::read(is,XR_);
    	itensor::read(is,il_);
    	itensor::read(is,ir_);
    	itensor::read(is,is_);
    	itensor::read(is,W_);
    	itensor::read(is,wl_);
    	itensor::read(is,wr_);
    	itensor::read(is,entropy_);
    	itensor::read(is,energy_);
    }

    void write(ostream& os) const // Poorly implemented...
    {
    	itensor::write(os,m_);
    	itensor::write(os,A_);
    	itensor::write(os,AL_);
    	itensor::write(os,AR_);
    	itensor::write(os,C_);
    	itensor::write(os,AC_);
    	itensor::write(os,lw_);
    	itensor::write(os,rw_);
    	itensor::write(os,L_);
    	itensor::write(os,R_);
    	itensor::write(os,XL_);
    	itensor::write(os,XR_);
    	itensor::write(os,il_);
    	itensor::write(os,ir_);
    	itensor::write(os,is_);
    	itensor::write(os,W_);
    	itensor::write(os,wl_);
    	itensor::write(os,wr_);
    	itensor::write(os,entropy_);
    	itensor::write(os,energy_);
    }

}; 

#endif