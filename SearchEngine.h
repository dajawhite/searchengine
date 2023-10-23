#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "DocParser.h"
#include "QueryProcessor.h"

using namespace std;

class SearchEngine {

public:

	SearchEngine();	// Default constructor

	int index(const string& path);
	void search(const string& query, vector<FileAVLData>& results) const;
	void write(ostream&);
	void read(istream&);

	

private:

	IndexHandler index_handler;
	DocParser doc_parser;
	QueryProcessor query_processor;

	struct FileReport {
		FileAVLData file_uuid_w_info;
		double total_tf_idf;
		bool operator<(const FileReport& rhs) const 
			{ return file_uuid_w_info.uuid < rhs.file_uuid_w_info.uuid; }
	};

	
};

#endif
