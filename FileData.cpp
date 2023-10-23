#include "FileData.h"
#include <iostream>

using namespace std;

std::ostream& operator<<(std::ostream& os, const FileAVLData& f_uuid_w_info) {
	os << f_uuid_w_info.uuid << endl;
	os << f_uuid_w_info.stats.title << endl;
	os << f_uuid_w_info.stats.filename << endl;
	os << f_uuid_w_info.stats.date_published << endl;
	os << f_uuid_w_info.stats.url << endl;
	os << f_uuid_w_info.stats.word_count << endl;
	return os;
}

std::istream& operator>>(std::istream& is, FileAVLData& f_uuid_w_info) {
	getline(is, f_uuid_w_info.uuid);
	getline(is, f_uuid_w_info.stats.title);
	getline(is, f_uuid_w_info.stats.filename);
	getline(is, f_uuid_w_info.stats.date_published);
	getline(is, f_uuid_w_info.stats.url);
	is >> f_uuid_w_info.stats.word_count;
	is.ignore();
	return is;
}
