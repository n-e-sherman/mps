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
		lOp = op(sites->getSites(),args->getString("localOperator"),c);
		auto temp = _res(c) * lOp; temp.noPrime();
		_res.set(c,temp);
		return State(_res);
	}

	virtual void updatePosition(int x)
	{
		c = x;
	}

	void read(istream& is)
    {
    	Operator::read(is);
    	itensor::read(is,lOp);
    	_build();
    }

    void write(ostream& os) const
    {
    	Operator::write(os);
    	itensor::write(os,lOp);
    }

protected:

	void _build()
	{
		auto thermal = args->getBool("thermal");
		auto N = args->getInt("N"); 
		c = ((thermal) ? N-1 : N/2);
	}


};
#endif