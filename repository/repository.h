#ifndef __REPOSITORY_H_
#define __REPOSITORY_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include <fstream>
#include <iterator>
#include <string>

using namespace itensor;
using namespace std;

class Repository
{
protected:
	string cwd;
	Cache* cache;
public:
	Repository() {}
	Repository(string c) : cwd(c) {cache = Cache::getInstance();}
	virtual ~Repository(){}

	template<class T> 
	T* load(const std::string& hash, T* T_in) 
	{	 
		auto retC = (T*)cache.load(fname);
		if(retC != nullptr) return retC;
		ifstream file;
		file.open(cwd+".data/"+hash, ios::in | ios::binary);
		if(file.is_open()) {T_in.load(file); }
		else {T_in = nullptr; }
	}
	template<class T>
	void save(const std::string& hash, T* t) 
	{ 
		cache.save(hash,t);
		ofstream file;
		file.open(cwd+ ".data/" + hash, ios::out | ios::binary);
		t->write(file);
	}
	template<class T,class LabelT>
	void save(const std::string &hash, const LabelT& labels, const T& data, string delimeter = ",")
	{
		/* This function assumes a 2D structure of iterables.
		 * example: vector<vector<.>>. The outer iterable are
		 * rows, and the inner iterable is a row.
		 */
		ofstream file;
		file.open(cwd + ".results/" + hash);
		auto it = labels.begin();
		while(it != labels.end())
		{
			file << *it;
			if(++it != row.end()) file << delimeter;
		}
		file << "\n";

		for(const auto & row : data)
		{
			auto it = row.begin();
			while(it != row.end())
			{
				if(*it != NAN) file << *it;
				if(++it != row.end()) file << delimeter;
			}
			file << "\n";
		}
		file.close();
	}


};

#endif
