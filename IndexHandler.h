#ifndef INDEX_HANDLER_H
#define INDEX_HANDLER_H

#include <iostream>
//#include <string>
//#include <vector>
#include "AVLTree.h"
#include "WordData.h"
#include "FileData.h"

using namespace std;

class IndexHandler {
	public:
		
		// Adds basic information for a single file to the indexer.
		IndexHandler();
		void insert_file_info(
		const string& uuid, const std::string& title,
		const string& filename, const std::string& date_published,
		const string& url, unsigned int text_word_count);

		//adds a word/org/person to the Set term.
		void insert_term(const string& term, const string& uuid); 
		void insert_org(const string& org, const string& uuid); //
		void insert_person(const string& person, const string& uuid);

		// Returns the information for a single file.
		const FileStatistic* lookup_file_info(const std::string& uuid) const;
		
		//	goes into the set, find the AVLNode with that term and returns stats.
		//	Returned vectors are sorted by uuid. Returns nullptr if search word not found.
		// Returns the statistics for a term/organization/person, that is, the uuids of all
		// of the files that contain the term/organization/person, as well as how many times
		// the word term/organization/person occured in those files.
		// Returned vectors are sorted by uuid. Returns nullptr if search word not found.
		vector<WordStatistic> lookup_term( const string& term) ;
		vector<WordStatistic> lookup_org( const string& org) ;
		vector<WordStatistic> lookup_person(const string& person) ;
			
		// Writes all indexed data to a persistence file in a format that can be quickly
		// loaded later (without having to reread json files).
		void write(std::ostream&);

		// Loads the indexed data in a file. The data was placed in the file using a call
		// to write().
		void read(std::istream&);

		unsigned int file_count() const { return files.size(); }

	private:
		/* These three structures are used to quickly (in logn time)
		* lookup the ids of the files that contain a
		* given search word, and also the frequency of the
		* word in the files that contain it. 
		*/
		AvlTree<WordAVLData> terms;	
		AvlTree<WordAVLData> orgs;		
		AvlTree<WordAVLData> persons;	

		/* This structure is used to quickly (in logn time)
		* lookup the full name of a file and its word count
		* if the file id is known.
		*/
		AvlTree<FileAVLData> files;	

		//helper functions 
		/* Adds a single word within a file to the specified AvlTree, which could be either
		of the trees for terms/organizations/persons.*/
		static void insert_word (const string& word, const string& uuid, AvlTree<WordAVLData>& dest); 
		
		/* Looks up a single word in the specified AvlTree, which could be either of the trees
		for terms/organizations/persons.*/
		static vector<WordStatistic> lookup_word(const string& word, AvlTree<WordAVLData>* src); 
};

#endif
