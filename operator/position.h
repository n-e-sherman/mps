#ifndef __POSITION_H_
#define __POSITION_H_

#include "operator/operator.h"

class Position : public Operator
{
protected:
	
	int c;
	ITensor lOp;

public:

	Position(Args* a, Sites* s) : Operator(a,s) { _build(); }

	virtual State multiply(State& s_in, Args* args_in = nullptr)
	{	
		auto _res = MPS(s_in.getState());
		auto psi = MPS(_res);
		auto temp = psi(c) * lOp; temp.noPrime();
		psi.set(c,temp);
		auto lOp_E = inner(_res,psi);
		auto _s = sites->getSites()(c);
	    auto sP = prime(_s);
	    auto Up = _s(1); auto UpP = sP(1); auto Dn = _s(2); auto DnP = sP(2);
	    auto lOp_c = ITensor(dag(_s),sP); lOp_c.set(Up,UpP,lOp_E); lOp_c.set(Dn,DnP,lOp_E);
	    auto lOp_T = lOp-lOp_c;
		temp = _res(c) * lOp_T; temp.noPrime();
		_res.set(c,temp);
		return State(_res);
	}

private:

	void _build()
	{
		auto thermal = args->getBool("thermal");
		auto N = args->getInt("N"); 
		c = ((thermal) ? N-1 : N/2);
		lOp = op(sites->getSites(),args->getString("localOperator"),c);
	}

};
#endif