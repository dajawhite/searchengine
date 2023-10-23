#ifndef DOC_PARSER_H
#define DOC_PARSER_H

#include <string>
#include "IndexHandler.h"

class DocParser {

public:

	// Constructor
	DocParser(IndexHandler* pindexer);

	// Destructor
	~DocParser();

	// Calls process_json() on all .json files in the folder having the specified url.
	// Returns the number of .json files successfully processed.
	int process_folder(const std::string& path);

private:

	IndexHandler* pindexer;	// DocParser does not own its IndexHandler resource.

	// Parses the text/organizations/persons in a single .json file and inserts the
	// data into the indexer.
	bool process_json(const std::string& path);
};

#endif

