#include <iostream>
#include <fstream>
#include <limits>
#include "SearchEngine.h"
#include "WordSanitizer.h"
using namespace std;

enum class MENU_OPTION { INDEX_DIR, WRITE_PERSIST, READ_PERSIST, QUERY, QUIT };

MENU_OPTION get_menu_option();
void index_directory(SearchEngine& engine);
void perform_query(const SearchEngine& engine);
void write_to_file(SearchEngine& engine);
void read_from_file(SearchEngine& engine);

int main() {

	SearchEngine engine;
	MENU_OPTION option = MENU_OPTION::QUIT;

	do {
		option = get_menu_option();

		switch (option)
		{
		case MENU_OPTION::INDEX_DIR:		index_directory(engine);	break;
		case MENU_OPTION::WRITE_PERSIST:	write_to_file(engine);		break;
		case MENU_OPTION::READ_PERSIST:		read_from_file(engine);		break;
		case MENU_OPTION::QUERY:			perform_query(engine);		break;
		case MENU_OPTION::QUIT:											break;
		default:	break;
		}
	} while (option != MENU_OPTION::QUIT);

	return 0;
}

MENU_OPTION get_menu_option()
{
	int response = -1;

	cin.clear();

	while (true) {
		cout << "\nSearch Engine Main Menu\n";
		cout << "=======================\n";
		cout << "1: Index new directory\n";
		cout << "2: Perform search query\n";
		cout << "3: Write database to persistence file\n";
		cout << "4: Load database from persistence file\n";
		cout << "5: Quit\n";
		cout << "Select one of the options above: ";
		if (cin >> response && response >= 1 && response <= 5)
			break;
		else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	if (response == 1)	return MENU_OPTION::INDEX_DIR;
	if (response == 2)	return MENU_OPTION::QUERY;
	if (response == 3)	return MENU_OPTION::WRITE_PERSIST;
	if (response == 4)	return MENU_OPTION::READ_PERSIST;
	return MENU_OPTION::QUIT;
}

void index_directory(SearchEngine& engine)
{
	string path;
	int json_count;

	cout << "\nEnter file path: ";
	cin.clear();
	getline(cin, path);

	json_count = engine.index(path);
	cout << "Successfully indexed " << json_count << " .json files.\n";
}

void perform_query(const SearchEngine& engine) {

	string query;
	std::vector<FileAVLData> results;

	cout << "\nEnter search query: ";
	cin.clear();
	getline(cin, query);

	engine.search(query, results);

	// Print results
	if (results.empty()) {
		std::cout << "\nNo matching files found.\n";
	}
	else {
		std::cout << "\nMatching files:\n";
		for (auto result : results) {
			std::cout << "\nUUID: " << result.uuid;
			std::cout << "\nTitle: " << result.stats.title;
			std::cout << "\nDate Published: " << result.stats.date_published;
			std::cout << "\nURL: " << result.stats.url;
			std::cout << "\nfilename: " << result.stats.filename;
			std::cout << "\nWord count: " << result.stats.word_count << '\n';
		}
	}

}

void write_to_file(SearchEngine& engine)
{
	string filename;
	cout << "\nEnter name of persistence file to overwrite: ";
	cin.clear();
	getline(cin, filename);

	ofstream fout;
	fout.open(filename);
	if (fout.is_open() == false) {
		cout << "\nUnable to open \"" << filename << "\".\n";
	}
	else {
		engine.write(fout);
	}
}

void read_from_file(SearchEngine& engine)
{
	string filename;
	cout << "\nEnter name of persistence file to load: ";
	cin.clear();
	getline(cin, filename);

	ifstream fin;
	fin.open(filename);
	if (fin.is_open() == false) {
		cout << "\nUnable to open \"" << filename << "\".\n";
	}
	else {
		engine.read(fin);
	}
}
