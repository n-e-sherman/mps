#ifndef __UTIL_H_
#define __UTIL_H_

#include <sstream> 
#include <sys/stat.h>
#include "itensor/all.h"

using namespace itensor;
using namespace std;

vector<Real>
stringToVector(string s)
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

vector<string>
splitString(string s)
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


bool 
pathExists(const std::string &s) 
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

vector<Real>
linspace(Real a, Real b, int n)
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

char*
stoc(std::string x)
{
    char *y = new char[x.length() + 1];
    std::strcpy(y, x.c_str());
    return y;
}



#endif