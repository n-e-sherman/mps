#ifndef __UTIL_H_
#define __UTIL_H_

#include <sstream> 
#include <sys/stat.h>
#include "itensor/all.h"
#include "itensor/util/print_macro.h"
#include <bits/stdc++.h> 

using namespace itensor;
using namespace std;

vector<Real> stringToVector(string s)
{   // I am assuming this string is comma separated, and NO SPACES
    stringstream ss(s);
    vector<string> sres;
    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );
        sres.push_back( substr );
    }
    vector<Real> res;
    for(auto x : sres) res.push_back(stod(x));
    return res;
}
vector<int> stringToVectorI(string s)
{   // I am assuming this string is comma separated, and NO SPACES
    stringstream ss(s);
    vector<string> sres;
    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );
        sres.push_back( substr );
    }
    vector<int> res;
    for(auto x : sres) res.push_back(stoi(x));
    return res;
}

vector<string> splitString(string s)
{   // I am assuming this string is comma separated, and NO SPACES
    stringstream ss(s);
    vector<string> sres;
    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );
        sres.push_back( substr );
    }
    return sres;
}

void prepare(MPS &a, MPS &b, IndexSet is)
{
    a.replaceSiteInds(is);
    b.replaceSiteInds(is);
}


bool pathExists(const std::string &s) 
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

vector<Real> linspace(Real a, Real b, int n)
{
    vector<Real> res;
    Real step = (b-a)/(n-1);
    if(n==1){
    	res.push_back(a);
    	return res;
    }
    for(int i = 0; i < n; i++)
    {
        res.push_back(a + step*i);
    }
    return res;
}

char* stoc(std::string x)
{
    char *y = new char[x.length() + 1];
    std::strcpy(y, x.c_str());
    return y;
}

Args* getArgs(int argc, char* argv[])
{
    Args* args = new Args();

    /********************************
     ******** Default values ********
     ********************************/

    /* Calculation parameters. */
    
    args->add("Chebyshev",false);
    args->add("Correlation",false);
    // args->add("Measure",false);
    // args->add("Broadening",false);
    // args->add("Moments",false);
    // args->add("Weights",false);

    /* IO */
    args->add("cwd","./");
    args->add("dataDir","./");
    args->add("resDir","./");


    /* Global parameters */
    args->add("thermal",true);
    args->add("N",100);
    args->add("SiteSet","SpinHalf");
    args->add("ConserveSz",true);
    args->add("ConserveQNs",true);
    args->add("imaginary",true);
    args->add("Normalize",false);
    args->add("read",true);
    args->add("write",true);


    /* State parameters. */
    args->add("thermalMaxDim",2000);
    args->add("realStep",true);
    args->add("beta",0);
    args->add("tau",0.1);
    args->add("thermalEps",1E-9);
    args->add("coolingType","Trotter");

    /* Operator parameters. */
    args->add("Operator","Momentum");
    args->add("localOperator","Sz");

    /* Measurement parameters. */
    args->add("Measurement","Connected");

    /* Model parameters. */
    args->add("Lattice","Chain");
    args->add("Model","Heisenberg");
    args->add("Delta",1.0);
    args->add("Je",1.0);
    args->add("Jo",1.0);
    args->add("Jz",1.0);
    args->add("Jxy",1.0);
    args->add("B",1.0);

    /* Evolver parameters. */
    args->add("Evolver","Trotter");
    args->add("tau",0.1);
    args->add("beta",0);
    args->add("time",0);

    /* Chebyshev parameters. */
    args->add("W",8);
    args->add("Wp",0.9875);
    args->add("nChebyshev",100);
    args->add("nSave",1); // Chebyshev iteration saving won't occur by default.
    args->add("momentum",false);
    args->add("saveChebyshev",true);
    args->add("loadChebyshev",true);
    args->add("cheReadFile",true);
    args->add("cheWriteFile",true);
    args->add("OpName","Sz");
    args->add("measureAll",false);
    args->add("Skip",true);

    /* Sweeper parameters. */
    args->add("Sweeper",false);
    args->add("sweeperType","identity");
    args->add("sweeperCount",10);
    args->add("phiThreshold",1E-12);
    args->add("MaxIter",30);
    args->add("Ep",1.0);
    args->add("NumCenter",2); // Also ITensor parameter.
    args->add("NormCutoff",1e-10); // Also ITensor parameter.
    args->add("difThreshold",1e-8);
    args->add("details",true);
    args->add("errorMPOProd",true);

    /* ITensor parameters. */
    args->add("nSweeps",5);
    args->add("sweeps_maxdim","80,100,150,150,200");
    args->add("sweeps_mindim","20,20,10,10,10");
    args->add("sweeps_cutoff","1E-6,1E-8,1E-10,1E-12,1E-12");
    args->add("sweeps_niter","4,3,2,2,2");
    args->add("sweeps_noise","1E-7,1E-8,1E-10,0,0");
    args->add("initial","AF");
    args->add("UseSVD",true);
    args->add("MaxIter",30);
    args->add("NormCutoff",1e-7); // Also Sweeper parameter.
    args->add("numCenter",1); // Also Sweeper parameter.
    args->add("ConserveQNs",true);
    args->add("MaxDim",500);
    args->add("MinDim",50);
    args->add("Method","DensityMatrix");
    args->add("Nsweep",3);
    
    /* Artifacts */
    args->add("Nmoments",0);
    args->add("wi",0);
    args->add("wf",4);
    args->add("nw",201);
    args->add("etas","0.1,0.2");
    args->add("eps",0.0001);
    args->add("c",1);
    args->add("nLanczos",40);
    args->add("reorthogonalize",false);
    args->add("spectral");
    args->add("squared",false);
    args->add("writeDirectory",""); // Potentially irrelevant...



    string eq = "=";
    string dash = "-";
    string ddash = "--";
    for(int i = 1; i < argc; i++)
    {
        auto arg = string(argv[i]);

        if(arg.find(ddash) != string::npos) // double dash argument
        {
            if(arg.find(eq) != string::npos) // double dash with equals
            {
                args->add(stoc(arg.erase(0,2)));
            }
            else // value in next arg or no value or space then equals.
            {
                if((i+1) >= argc)
                {
                    args->add(stoc(arg.erase(0,2)));
                    break;
                }
                if(string(argv[i+1]).find(eq) != string::npos) // space then equals
                {
                    if((i+2) >= argc)
                    {
                        args->add(stoc(arg.erase(0,2)));
                        break;
                    }
                    args->add(stoc(arg.erase(0,2) + string(argv[i+1]) + string(argv[i+2]) ));
                    i += 2;
                }
                if(string(argv[i+1]).find(ddash) != string::npos) // no value
                    args->add(stoc(arg.erase(0,2)));
                else
                    args->add(stoc(arg.erase(0,2) + "=" + string(argv[++i])));
            }
        }

        else
        if(arg.find(dash) != string::npos) // dash argument
        {
            if(arg.find(eq) != string::npos) // dash with equals
            {
                args->add(stoc(arg.erase(0,1)));
            }
            else // value in next arg or no value or space then equals
            {
                if((i+1) >= argc)
                {
                    args->add(stoc(arg.erase(0,1)));
                    break;
                }
                if(string(argv[i+1]).find(eq) != string::npos) // space then equals
                {
                    if((i+2) >= argc)
                    {
                        args->add(stoc(arg.erase(0,1)));
                        break;
                    }
                    args->add(stoc(arg.erase(0,1) + string(argv[i+1]) + string(argv[i+2]) ));
                    i += 2;
                }
                if(string(argv[i+1]).find(dash) != string::npos) // no value
                    args->add(stoc(arg.erase(0,1)));
                else
                    args->add(stoc(arg.erase(0,1) + "=" + string(argv[++i])));
            }
        }

        else
        if(arg.find(eq) != string::npos) // dash with equals
        {
            args->add(stoc(arg));
        }
        else // word with no equals
        {

            if((i+1) >= argc)
            {
                args->add(stoc(arg));
                break;
            }
            if(string(argv[i+1]).find(eq) != string::npos) // space then equals
            {
                if((i+2) >= argc)
                {
                    args->add(stoc(arg));
                    break;
                }
                args->add(stoc(arg + string(argv[i+1]) + string(argv[i+2]) ));
                i += 2;
            }
            else
            {
                args->add(stoc(arg));
            }
        }


    } 
    if(!args->defined("position"))
        if(!args->defined("qFactor")) args->add("qFactor",1.0);
    return args;
}

struct StringReal
{
    enum type
    {
        tReal = 0,
        tString = 1,
        tCplx = 2
    };
    string s = "";
    Real r = NAN;
    Cplx c = NAN;
    type t = tReal;
    StringReal(){}
    StringReal(Cplx in)
    {
        t = tCplx;
        c = in;
    }
    StringReal(Real in)
    {
        t = tReal;
        r = in;
    }
    StringReal(char* in)
    {
        t = tString;
        s = string(in);
    }
    StringReal(string in)
    {
        t = tString;
        s = in;
    }

     Cplx Complex()
    {
        if(t == tReal) return Cplx(r);
        else
        if(t == tCplx) return c;
        else
                       return NAN;
    }

    Real real()
    {
        if(t == tReal) return r;
        else
        if(t == tCplx) return c.real();
        else
                       return NAN;

    }

    Real imag()
    {
        if(t == tReal) return 0;
        else
        if(t == tCplx) return c.imag();
        else
                       return NAN;

    }

    template<class T>
    StringReal operator=(const T& t) {return StringReal(t);}

    template<class T>
    bool operator==(T& t_in)
    {
        if((*this).t == tReal) {return ((*this).r == t_in); }
        else
        if((*this).t == tCplx) {return ((*this).c == t_in); }
        else
        if((*this).t == tString) {return (*this).s == t_in; }
    }
    friend std::ostream& operator<<( ostream& os, const StringReal& sr) 
    {
        if(sr.t == tReal){ os << sr.r; }
        else
        if(sr.t == tCplx){ os << sr.c; }
        else
        if(sr.t == tString){ os << sr.s; }
        return os; 
    }
    void read(istream& is)
    {
        itensor::read(is,t);
        itensor::read(is,s);
        itensor::read(is,r);
        itensor::read(is,c);
    }
    void write(ostream& os) const
    {
        itensor::write(os,t);
        itensor::write(os,s);
        itensor::write(os,r);
        itensor::write(os,c);
    }

};



#endif