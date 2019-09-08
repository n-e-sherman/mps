#ifndef __REPOSITORY_H_
#define __REPOSITORY_H_

#include <string>
#include "itensor/all.h"
#include "infrastructure/util.h"
#include <fstream>
#include <iterator>

using namespace itensor;

class Repository
{

public:
	Repository(){}
	virtual ~Repository(){}

	template<class T> 
	T read(const std::string& fname) { return readFromFile<T>(fname); }
	template<class T>
	void writeData(const std::string& fname, const T& t) { writeToFile<T>(".data/"+fname,t); }
	template<class T>
	void writeResult(const std::string &s,const T& data)
	{
		/* This function assumes a 2D structure of iterables.
		 * example: vector<vector<.>>. The outer iterable are
		 * rows, and the inner iterable is a row.
		 */
		auto fname = ".results/"+s;
		std::fstream file;
		file.open(fname);
		for(const auto & row : data)
		{
			auto it = row.begin();
			while(it != row.end())
			{
				file << *it;
				if(++it != row.end()) file << ",";
			}
			file << "\n";
		}
		file.close();
	}


};

#endif
