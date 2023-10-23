#include "SearchEngine.h"
#include <algorithm>	// For lower_bound
#include <iostream>
#include <cassert>
#include <cmath>

SearchEngine::SearchEngine()
	:	index_handler(),
		doc_parser(&index_handler), 
		query_processor(&index_handler)
{
}

int SearchEngine::index(const string& path)
{
	return doc_parser.process_folder(path);
}

void SearchEngine::search (const string& query, vector<FileAVLData>& ranked_results) const 
{
	const vector<WordStatistic> results = query_processor.get_results(query);

	// Sorted by uuid
	vector<FileReport> matching_files;	

	// N: number of files in index handler
	unsigned int N = index_handler.file_count();

	// Calculate the tf-idf statistic for each result
	for (auto results_for_word : results) {
		const FileStatistic* pfile_info = index_handler.lookup_file_info(results_for_word.uuid);

		assert(pfile_info != nullptr);

		FileReport file_report;
		file_report.file_uuid_w_info.uuid = results_for_word.uuid;

		auto it = lower_bound(matching_files.begin(), matching_files.end(), file_report);

		if (it == matching_files.end() || it->file_uuid_w_info.uuid != results_for_word.uuid) {
			file_report.file_uuid_w_info.stats = *pfile_info;
			file_report.total_tf_idf = 0;
			it = matching_files.insert(it, file_report);
		}

		// Calculate tf-idf for the word-file combination and add it to the
		// total tf-idf for the file.
		it->total_tf_idf +=
			(double(results_for_word.term_freq) / pfile_info->word_count) *
			::log10(double(N) / results.size()); //<-- ig this is supposed to be result instead of results_for_word size now eek!
	}

	// Sort results files in descending order based on total tf-idf
	sort(matching_files.begin(), matching_files.end(), 
	[](const FileReport& a, const FileReport& b) { return a.total_tf_idf > b.total_tf_idf; }
	);

	// Extract the file info for the ranked results (discarding the tf-idf)
	ranked_results.clear();
	for (auto& file_report : matching_files) {
		ranked_results.push_back(file_report.file_uuid_w_info);
	}
}

void SearchEngine::write(ostream& os)
{
	index_handler.write(os);
}

void SearchEngine::read(istream& is)
{
	index_handler.read(is);
}
