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
	string dataDir;
	string resultDir;
	Cache* cache;
	bool readFile = false;
	bool writeFile = false;

public:

	Repository(string d, string r, bool rf = false, bool wf = false) : dataDir(d), resultDir(r) 
	{
		cache = Cache::getInstance(); 
		readFile = rf; 
		writeFile = wf; 
	}

	template<class T> 
	T* load(const std::string& hash, T* T_in, bool loadFile = true) 
	{	
		cwd = dataDir;
		auto retC = (T*)cache->load(hash);
		if(retC != nullptr) return retC;
		if(loadFile & readFile)
		{
			ifstream file;
			file.open(cwd+".data/"+hash, ios::in | ios::binary);
			if(file.is_open()) {T_in->load(file); }
			else {T_in = nullptr; }
			return T_in;
		}
		else
			return nullptr;
	}

	template<class T>
	void save(const std::string& hash, T* t, bool saveFile = true) 
	{ 
		cwd = dataDir;
		cache->save(hash,t);
		if(saveFile & writeFile)
		{
			ofstream file;
			file.open(cwd+ ".data/" + hash, ios::binary | ofstream::trunc);
			t->save(file);
			file.close();
		}
	}

	template<class T,class LabelT>
	void save(const std::string &hash, const std::string& folder, const LabelT& labels, const T& data, bool print_location = false)
	{
		/* This function assumes a 2D structure of iterables.
		 * example: vector<vector<.>>. The outer iterable are
		 * which row, and the inner iterable is a row.
		 */
		cwd = resultDir;
		ofstream file;
		if (print_location)
			cout <<  "Results saved at location: " << cwd + ".results/" + folder + "/" + hash << endl;
		file.open(cwd + ".results/" + folder + "/" + hash);
		auto it = labels.begin();
		while(it != labels.end())
		{
			file << *it;
			if(++it != labels.end()) file << ",";
		}
		file << "\n";

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
