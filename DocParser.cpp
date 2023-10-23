#include "DocParser.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
//#include "porter_stemmer/porter2_stemmer.h"
#include "WordSanitizer.h"

// RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

DocParser::DocParser(IndexHandler* pindexer)
    : pindexer(pindexer)
{
}

DocParser::~DocParser()
{
    pindexer = nullptr;
}

int DocParser::process_folder(const std::string& path)
{
    int success_count = 0; //counter for number of succesfully processed file

    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator so it will go into subfolders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(path); //look all files in a folder

    // loop over all the entries.
    for (const auto& entry : it) {
        // We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json") {
            if (process_json(entry.path().string())) {
                success_count++;
            }
        }
    }

    return success_count;
}

bool DocParser::process_json(const std::string& path)
{
    // open an ifstream on the file of interest and check that it could be opened.
    ifstream input(path);
    if (!input.is_open()) {
        cerr << "cannot open file: " << path << endl;
        return false;
    }
    
    // Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);

    // Now that the document is parsed, we can access different elements the JSON using
    // familiar subscript notation.

    // Get the document id and title
    auto uuid = d["uuid"].GetString();
    auto title = d["title"].GetString();
    auto date = d["published"].GetString();
    auto url = d["url"].GetString();
    auto len = d["text"].GetStringLength(); // This will include stopwords (todo should we include them?)

    // Copy the "text" of the json file into an std::string
    std::string full_text;
    full_text.reserve(len);
    for (auto ptext = d["text"].GetString(); *ptext != '\0'; ptext++) {
        full_text.push_back(*ptext);
    }

    // Index the text of the document
    std::istringstream iss(full_text);
    std::string word;
    while (iss >> word) {

        // Sanitize each word
        WordSanitizer::sanitize(word);

        // If word is not a stopword, then hand it to the indexer
        if (!word.empty() && !WordSanitizer::is_stopword(word)) {
            pindexer->insert_term(word, uuid);
        }
    }

    // Index the file info of the document
    pindexer->insert_file_info(uuid, title, path, date, url, len);

    // Index the organizations of the document
    auto orgs = d["entities"]["organizations"].GetArray();
    for (auto& p : orgs) {
        iss.clear();
        iss.str(p["name"].GetString());
        while (iss >> word) {
            
            // Sanitize each word
            WordSanitizer::sanitize(word);

            // If word is not a stopword, then hand it to the indexer
            if (!word.empty() && !WordSanitizer::is_stopword(word)) {
                pindexer->insert_org(word, uuid);
            }
        }
    }

    // Index the persons of the document
    auto persons = d["entities"]["persons"].GetArray();
    for (auto& p : persons) {
        iss.clear();
        iss.str(p["name"].GetString());
        while (iss >> word) {
            
            // Sanitize each word
            WordSanitizer::sanitize(word);

            // Stopwords are okay for person names
            if (!word.empty()) {
                pindexer->insert_person(word, uuid);
            }
        }
    }

    input.close();

    return true;
}

