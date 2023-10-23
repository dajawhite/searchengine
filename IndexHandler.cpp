#include <algorithm>	// For std::lower_bound
#include "IndexHandler.h"

IndexHandler::IndexHandler(){}

void IndexHandler::insert_file_info(
	const std::string& uuid, const std::string& title,
	const std::string& filename, const std::string& date_published,
	const std::string& url, unsigned int text_word_count)
{
	FileAVLData file_avl_data;
	file_avl_data.uuid = uuid;
	file_avl_data.stats.title = title;
	file_avl_data.stats.filename = filename;
	file_avl_data.stats.date_published = date_published;
	file_avl_data.stats.url = url;
	file_avl_data.stats.word_count = text_word_count;

	// Check if file uuid has already been added to set
	if (files.contains(file_avl_data) == false) {	// uuid not found
		// Insert the file data into the set
		files.insert(file_avl_data);
	}
	else {	// uuid already inserted (so do nothing)
	}
}

//calls insert_word to insert a term into the term index
void IndexHandler::insert_term(const string& term, const string& uuid)
{
	insert_word(term, uuid, terms);
}

//calls insert_word to insert an org into the org index
void IndexHandler::insert_org(const string& org, const string& uuid)
{
	insert_word(org, uuid, orgs);
}

//calls insert_word to insert a person into the persons index
void IndexHandler::insert_person(const string& person, const string& uuid)
{
	insert_word(person, uuid, persons);
}

//WHAT IS THIS FUNCTION FOR??
const FileStatistic* IndexHandler::lookup_file_info(const std::string& uuid) const
{
	// todo
	return nullptr;
}

//calls lookup_word to lookup a term
vector<WordStatistic> IndexHandler::lookup_term(const string& term)
{
	return lookup_word(term, &terms);
}

//calls lookup_word to lookup an org
vector<WordStatistic> IndexHandler::lookup_org(const string& org)
{
	return lookup_word(org, &orgs);
}


//calls lookup_word to lookup a person
vector<WordStatistic> IndexHandler::lookup_person(const string& person)
{
	return lookup_word(person, &persons);
}

//IndexHandler needs to know when to read and write
//It tells each one of the AVLTrees when to read and write
//Search Engine will be calling those.

void IndexHandler::write(std::ostream& os)
{
	os << terms.size() << '\n';
	terms.write(os);

	os << orgs.size() << '\n';
	orgs.write(os);

	os << persons.size() << '\n';
	persons.write(os);

	os << files.size() << '\n';
	files.write(os);
}

void IndexHandler::read(std::istream& is)
{
	unsigned int size;

	is >> size;
	is.ignore();
	terms.read(is, size);
	
	is >> size;
	is.ignore(); 
	orgs.read(is, size);
	
	is >> size;
	is.ignore(); 
	persons.read(is, size);
	
	is >> size;
	is.ignore(); 
	files.read(is, size);
}

 void IndexHandler::insert_word(
	const string& word, const string& uuid, AvlTree<WordAVLData>& dest)
{
	WordAVLData newNode; //an instance of WordAVLData
	newNode.word = word; //set that instance to the word argument

	// Check if word has already been added to the tree
	// it is a comparable
	 //dest = the tree. Looks for the data in the tree.

	if(!dest.contains(newNode)){
		newNode.addDoc(uuid,1); 
	}
	else{
		// word found
		// Extract the word data from the tree (and remove from tree)
		auto it = dest[newNode];
		newNode.stats = it.stats; //make a copy of the word and its statistics
		dest.remove(it); //move it temporarely from the tree
		// Increase the word-frequency for the word/file combo
		WordStatistic word_stat; //here is the statistics for the word
		word_stat.uuid = uuid; //
		word_stat.term_freq = 1;
		auto vit = std::find(newNode.stats.begin(), newNode.stats.end(), word_stat); //finding the statistics within the newNode.
		if (vit == newNode.stats.end()){ //|| vit->uuid != uuid took out cause it means same thing
			newNode.stats.insert(vit, word_stat); //add statistics to the word in case it has none for that specific uuid
		}
		else { //!(vit == newNode.stats.end()) || vit->uuid == uuid if i'm not at the end it means that I found it 
			vit->term_freq++; //if it already has some statistics for that specific uuid, then add one more.
		}
	}

	// Insert(or reinsert) the word data into the tree
	dest.insert(newNode); //inserting it back.
}

//had to make the AVLTree not const
vector<WordStatistic> IndexHandler::lookup_word(
	const string& word, AvlTree<WordAVLData>* src)
{
	WordAVLData wanted; 
	wanted.word = word;
	//cout<<wanted;

	// Check if word has already been added to tree
	auto it = (*src)[wanted]; //take src (entire AVLTree) and finds wanted

	/*if (it != wanted) {
		return nullptr;	// search word not found
	}*/

	return it.stats;
}
