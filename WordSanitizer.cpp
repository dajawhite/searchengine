#include <algorithm>
#include <cassert>
#include <cctype>
#include "WordSanitizer.h"
#include "porter_stemmer/porter2_stemmer.h"

const std::string WordSanitizer::stopwords[] = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being",
    "below", "between", "both", "but", "by", "can't", "cannot", "could", "couldn't",
    "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down", "during",
    "each", "few", "for", "from", "further", "had", "hadn't", "has", "hasn't", "have",
    "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers",
    "herself", "him", "himself", "his", "how", "how's", "i", "i'd", "i'll", "i'm",
    "i've", "if", "in", "into", "is", "isn't", "it", "it's", "its", "itself", "let's",
    "me", "more", "most", "mustn't", "my", "myself", "no", "nor", "not", "of", "off",
    "on", "once", "only", "or", "other", "ought", "our", "ours", "ourselves", "out",
    "over", "own", "same", "shan't", "she", "she'd", "she'll", "she's", "should",
    "shouldn't", "so", "some", "such", "than", "that", "that's", "the", "their",
    "theirs", "them", "themselves", "then", "there", "there's", "these", "they",
    "they'd", "they'll", "they're", "they've", "this", "those", "through", "to", "too",
    "under", "until", "up", "very", "was", "wasn't", "we", "we'd", "we'll", "we're",
    "we've", "were", "weren't", "what", "what's", "when", "when's", "where", "where's",
    "which", "while", "who", "who's", "whom", "why", "why's", "with", "won't", "would",
    "wouldn't", "you", "you'd", "you'll", "you're", "you've", "your", "yours",
    "yourself", "yourselves"
};

const size_t WordSanitizer::stopwords_count = sizeof(stopwords) / sizeof(stopwords[0]);

void WordSanitizer::sanitize(std::string& word)
{
    remove_punc(word);
    tolower(word);
    Porter2Stemmer::stem(word);
}

bool WordSanitizer::is_stopword(const std::string& word)
{
    return std::binary_search(stopwords, stopwords + stopwords_count, word);
}

void WordSanitizer::remove_punc(std::string& word)
{
    auto it = std::remove_if(word.begin(), word.end(),
        [](char c) { return (c != '\'' && 
            !('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9')); }
    );
    word.resize(it - word.begin());
}

void WordSanitizer::tolower(std::string& word)
{
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}
