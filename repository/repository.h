#ifndef __REPOSITORY_H_
#define __REPOSITORY_H_

#include "itensor/all.h"
#include "infrastructure/util.h"
#include "infrastructure/cache.h"
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
		auto retC = (T*)cache->load(hash);
		if(retC != nullptr) return retC;
		ifstream file;
		file.open(cwd+".data/"+hash, ios::in | ios::binary);
		if(file.is_open()) {T_in->load(file); }
		else {T_in = nullptr; }
		return T_in;
	}
	template<class T>
	void save(const std::string& hash, T* t) 
	{ 
		cache->save(hash,t);
		ofstream file;
		file.open(cwd+ ".data/" + hash, ios::out | ios::binary);
		t->save(file);
	}
	template<class T,class LabelT>
	void save(const std::string &hash, const std::string& folder, const LabelT& labels, const T& data, string delimeter = ",")
	{
		/* This function assumes a 2D structure of iterables.
		 * example: vector<vector<.>>. The outer iterable are
		 * rows, and the inner iterable is a row.
		 */
		ofstream file;
		file.open(cwd + ".results/" + folder + "/" + hash);
		auto it = labels.begin();
		while(it != labels.end())
		{
			file << *it;
			if(++it != labels.end()) file << delimeter;
		}
		file << "\n";

		for(const auto & row : data)
		{
			auto it = row.begin();
			while(it != row.end())
			{
				file << *it;
				// if(!(*it == NAN)) file << *it;
				if(++it != row.end()) file << delimeter;
			}
			file << "\n";
		}
		file.close();
	}


};

#endif
