#ifndef __POSITIONCONNECTED_H_
#define __POSITIONCONNECTED_H_

#include "operator/operator.h"
#include "operator/position.h"

class PositionConnected : public Position
{
protected:
	
	ITensor lOp_T;
public:

	PositionConnected(Args* a, Sites* s) : Position(a,s) { _build(); }

	virtual State multiply(State& s_in, Args* args_in = nullptr)
	{	
		auto _res = MPS(s_in.getState());
		auto psi = MPS(_res);
		lOp = op(sites->getSites(),args->getString("localOperator"),c);
		auto temp = psi(c) * lOp; temp.noPrime();
		psi.set(c,temp);
		auto lOp_E = innerC(_res,psi).real(); // May want to generalize.
		auto _s = sites->getSites()(c);
	    auto sP = prime(_s);
	    auto Up = _s(1); auto UpP = sP(1); auto Dn = _s(2); auto DnP = sP(2);
	    auto lOp_c = ITensor(dag(_s),sP); lOp_c.set(Up,UpP,lOp_E); lOp_c.set(Dn,DnP,lOp_E);
	    lOp_T = lOp-lOp_c;
		temp = _res(c) * lOp_T; temp.noPrime();
		_res.set(c,temp);
		return State(_res);
	}

	virtual void updatePosition(int x)
	{
		c = x;
	}

	void read(istream& is)
    {
    	Position::read(is);
    	_build();
    }

    void write(ostream& os) const
    {
    	Position::write(os);
    }

protected:

};
#endif