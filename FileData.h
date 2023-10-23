#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <string>

// This structure stores essential information for a single file.
struct FileStatistic {
	std::string title;			// The "title" field from json file.
	std::string filename;		// The filename that has .json extension.
	std::string date_published;	// The "published" field from json file.
	std::string url;			// The webpage where the article was published,
								// which is the "url" field from json file.
	unsigned int word_count;	// Total number of words in the file. Need to record this to
								// calculate tf statistic without having to reread files.
};

// This structure is used by the avl tree to pair each file uuid with the
// other information about the file.
struct FileAVLData {
	std::string uuid;		// key
	FileStatistic stats;	// mapped value
	bool operator<(const FileAVLData& rhs) const {
		return uuid < rhs.uuid;
	}	// Needed by AVL tree to sort items in correct order.
};

std::ostream& operator<<(std::ostream&, const FileAVLData&);
std::istream& operator>>(std::istream&, FileAVLData&);

#endif
