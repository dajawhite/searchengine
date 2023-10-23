#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "IndexHandler.h"
#include <unordered_set>
using namespace std;

class QueryProcessor {

private:
	IndexHandler* pindexer;


public:
	QueryProcessor(IndexHandler* pindexer);
	~QueryProcessor();

	vector<WordStatistic> get_results(const string&) const;


};

#endif



