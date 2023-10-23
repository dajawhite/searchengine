#include "QueryProcessor.h"
#include "WordSanitizer.h"
#include <stack>
#include "string.h"
#include <sstream>


QueryProcessor::QueryProcessor(IndexHandler* pindexer)
	: pindexer(pindexer)
{
}

QueryProcessor::~QueryProcessor()
{
	pindexer = nullptr; 
}

 vector<WordStatistic> QueryProcessor::get_results(const string& query) const{
	//map: uuid, vector of tfidfs (update tfidfs), sort based on sum total of tf/idfs
		//> < overload to sort vectors by tfidf
	//unordered set for results
	unordered_set<WordStatistic, WordStatistic::MyHashFunction> tmpResults;
	unordered_set<WordStatistic, WordStatistic::MyHashFunction> notResults;
	
	//parse the query, sanitize, lookup
	string token;
	stringstream ss(query);
	while (getline(ss, token, ' ')){
		if(!WordSanitizer::is_stopword(token)){
			if (token.find("PERSON:") != string::npos){
				size_t pos = token.find(":");
				token = token.substr(pos + 1);
				WordSanitizer::sanitize(token);
				vector<WordStatistic> temp = pindexer->lookup_person(token);
				tmpResults.insert(temp.begin(), temp.end());
			}
			else if (token.find("ORG:") != string::npos){
				size_t pos = token.find(":");
				token = token.substr(pos + 1);
				WordSanitizer::sanitize(token);
				vector<WordStatistic> temp = pindexer->lookup_org(token);
				tmpResults.insert(temp.begin(), temp.end());
			}
			else if (token.find("-") != string::npos){
				size_t pos = token.find("-");
				token = token.substr(pos + 1);
				WordSanitizer::sanitize(token);
				vector<WordStatistic> temp = pindexer->lookup_term(token);
				notResults.insert(temp.begin(), temp.end());
			}
			else {
				WordSanitizer::sanitize(token);
				vector<WordStatistic> temp = pindexer->lookup_term(token);
				tmpResults.insert(temp.begin(), temp.end());
			}
		}
		else{}
	}

	for(auto& toRemove : notResults){
		tmpResults.erase(toRemove);
	}
	

	return vector<WordStatistic>(tmpResults.begin(), tmpResults.end());
	//return tmpResults;
}
