#ifndef __UTIL_H_
#define __UTIL_H_

#include <sstream> 
#include <sys/stat.h>
#include "itensor/all.h"
#include "itensor/util/print_macro.h"
#include "infrastructure/cache.h"
#include "infrastructure/defaults.h"
#include <bits/stdc++.h> 
#include "model/model.h"

using namespace itensor;
using namespace std;

vector<Real> stringToVector(string s)
{   // I am assuming this string is comma separated, and NO SPACES
    if (s == "") // empty string
        return vector<Real>();

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
    if (s == "") // empty string
        return vector<int>();

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
   
void print_matrix(vector<vector<int>> adjmat)
{
    for(auto i : range(adjmat.size()))
    {
        for(auto j : range(adjmat[i].size())) {cout << adjmat[i][j] << " ";}
        cout << endl;
    }
}

void print(vector<int> v, bool end = true)
{
    cout << "[";
    for(auto x : v) cout << x << ",";
    cout << "]";
    if (end)
        cout << endl;
}

void print(vector<vector<int>> v)
{
    cout << "[";
    for(auto x : v) print(x);
    cout << "]" << endl;
}

void print(set<int> v)
{
    cout << "{";
    for(auto x : v) cout << x << ",";
    cout << "}" << endl;
}

string _hash_string(string name, Args* args)
{
    auto h = args->getString(name); // will be NAN if a string.
    if(name == "Model") return "_" + args->getString(name) + "_" + Model::hashParams(args);
    return "_" + h;
}

string hash_string(vector<string> hashes, Args* args)
{
    string hash = "";
    for(auto &name : hashes) hash += _hash_string(name,args);   
    return hash;
}


string _hash_real(string name, Args* args)
{
    auto h = args->getReal(name); // will be NAN if a string.
    if(name == "M"){if (args->getString("initial") != "M") return"";}
    if((name == "beta")) { if(args->getBool("thermal")) return "_" + to_string(h); else {return "";} }
    if((name == "qx") || (name == "qy") || (name == "qz")) {if(args->getBool("momentum")) return "_" + to_string(h); else{return "";} }
    return "_" + to_string(h);
}

string hash_real(vector<string> hashes, Args* args)
{
    string hash = "";
    for(auto &name : hashes) hash += _hash_real(name,args);   
    return hash;
}

string _hash_bool(string name, Args* args)
{
    auto h = args->getBool(name); // will be NAN if a string.
    if(h)
        return "_" + to_string(h);
    return string("");
}

string hash_bool(vector<string> hashes, Args* args)
{
    string hash = "";
    for(auto &name : hashes) hash += _hash_real(name,args);   
    return hash;
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

    void print()
    {
        if(t == tReal)    cout << r << endl;
        else
        if(t == tCplx)    cout << c << endl;
        else
        if (t == tString) cout << s << endl;
        else
                          cout << NAN << endl;
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
        if((*this).t == tString) {return ((*this).s == t_in); }
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

/*************************************************************************/
/*************************************************************************/

void read_args(int argc, char* argv[], std::map<std::string, Args*>& args)
{
    /* Have tested multiple values for --<name>=<value> format. */
    /* assumes the command line argument has --<key>:<name>=<value> format. The # of dashes can be 0,1, or 2. */
    string comma = ",";
    string eq = "=";
    string dash = "-";
    string ddash = "--";
    string colon = ":";
    for(int i = 1; i < argc; i++)
    {
        auto arg = string(argv[i]);
        if(arg.find(ddash) != string::npos) // ddash format
        {
            auto sarg = arg.erase(0,2);
            if(arg.find(eq) != string::npos)
            {
                string arg_key = "global";
                if(sarg.find(colon) != string::npos) // we have a key
                {
                    arg_key = sarg.substr(0, sarg.find(colon));
                    sarg.erase(0,sarg.find(colon)+1);
                }
                if(arg.find(comma) != string::npos) // There are multiple values
                {
                    auto arg_name = sarg.substr(0, sarg.find(eq));
                    auto arg_value = sarg.substr(sarg.find(eq)+1,sarg.size());
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(arg_name, arg_value);
                    else
                        args[arg_key] = new Args(arg_name, arg_value);
                }
                else
                {
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(stoc(sarg));
                    else
                        args[arg_key] = new Args(stoc(sarg));
                }
            }
        }
        else
        if(arg.find(dash) != string::npos) // dash format
        {
            auto sarg = arg.erase(0,1);
            if(arg.find(eq) != string::npos)
            {
                string arg_key = "global";
                if(sarg.find(colon) != string::npos) // we have a key
                {
                    arg_key = sarg.substr(0, sarg.find(colon));
                    sarg.erase(0,sarg.find(colon)+1);
                }
                if(arg.find(comma) != string::npos) // There are multiple values
                {
                    auto sarg = arg.erase(0,1);
                    auto arg_name = sarg.substr(0, sarg.find(eq));
                    auto arg_value = sarg.substr(sarg.find(eq)+1,sarg.size());
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(arg_name, arg_value);
                    else
                        args[arg_key] = new Args(arg_name, arg_value);
                }
                else
                {
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(stoc(sarg));
                    else
                        args[arg_key] = new Args(stoc(sarg));
                }
            }
        }
        else
        if(arg.find(eq) != string::npos) // no dash format
        {
            auto sarg = arg;
            string arg_key = "global";
            if(sarg.find(colon) != string::npos) // we have a key
            {
                arg_key = sarg.substr(0, sarg.find(colon));
                sarg.erase(0,sarg.find(colon)+1);
            }
            if(arg.find(comma) != string::npos) // There are multiple values
                {
                    auto arg_name = sarg.substr(0, sarg.find(eq));
                    auto arg_value = sarg.substr(sarg.find(eq)+1,sarg.size());
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(arg_name, arg_value);
                    else
                        args[arg_key] = new Args(arg_name, arg_value);
                }
                else
                {
                    if(args.find(arg_key) != args.end()) 
                        args[arg_key]->add(stoc(sarg));
                    else
                        args[arg_key] = new Args(stoc(sarg));
                }
            
        }
        else // word with no equals
        {
            if((i+1) >= argc)
            {
                args["global"]->add(stoc(arg));
                // break;
            }
        }
    } 
}

Args* getArgs(int argc, char* argv[])
{
    std::map<std::string, Args*> args;
    args["global"] = default_global();
    args["chebyshevService.chebyshev"] = new Args("Skip",true);

    read_args(argc, argv, args);
    auto cache = Cache::getInstance();
    for (auto const& [key, val] : args)
    {
        cout << key << endl;
        cache->save(key,val);
    }
    return args["global"];
}

Args* build_args(Args* args_in, std::string base, std::string key = "NULL")
{
    auto cache = Cache::getInstance();
    auto args_base = (Args*)cache->load(base);
    auto args_key = (Args*)cache->load(key);
    auto args = new Args(*args_in);
    if (args_base != nullptr) (*args) += (*args_base); 
    if (args_key != nullptr) (*args) += (*args_key);
    return args;
}

    
#endif
