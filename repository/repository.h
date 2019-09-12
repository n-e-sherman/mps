#ifndef __REPOSITORY_H_
#define __REPOSITORY_H_

#include <string>
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
	T* load(const std::string& hash) 
	{	 
		auto ret = cache.get(fname);
		if(ret != nullptr) return (T*)cache;
		readFromFile<T>(fname); 
	}
	template<class T>
	void save(const std::string& hash, const T& t) { writeToFile<T>(cwd".data/"+fname,t); }
	template<class T,class LabelT>
	void write(const std::string &s, const LabelT& labels, const T& data, string delimeter = ",")
	{
		/* This function assumes a 2D structure of iterables.
		 * example: vector<vector<.>>. The outer iterable are
		 * rows, and the inner iterable is a row.
		 */
		auto fname = ".results/"+s;
		std::fstream file;
		file.open(fname);
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
				file << *it;
				if(++it != row.end()) file << delimeter;
			}
			file << "\n";
		}
		file.close();
	}


};

#endif
