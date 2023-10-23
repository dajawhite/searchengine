#ifndef WORD_SANITIZER_H
#define WORD_SANITIZER_H

#include <string>

class WordSanitizer {
public:

    static void sanitize(std::string& word);
        // Clean up the word by removing non-alphanumerical letters (except
        // apostrophe), converting to lowercase, and stemming.

    static bool is_stopword(const std::string& word);
        // Returns true if and only if word is a stopword (like "the").

private:

    static const std::string stopwords[];
    static const size_t stopwords_count;

    static void remove_punc(std::string& word);
    static void tolower(std::string& word);

};

#endif
