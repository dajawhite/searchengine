#ifndef WORD_DATA_H
#define WORD_DATA_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

using namespace std;


/* The id of a file.
* The number of times a specific word appears in the file.
* Need to record this to calculate tf statistic without
* having to reread files.
*/
struct WordStatistic {
    string uuid;	
    int term_freq;

    bool operator==(const WordStatistic& rhs) const{
        return uuid == rhs.uuid;
    }
    
    //https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-user-defined-class-in-cpp/
    struct MyHashFunction {
        size_t operator()(const WordStatistic& p) const
        {
            return hash<string>()(p.uuid);
        }
    };
};

// mapped value. Vector is sorted by uuid. Stores all the statistics
//(documents that contains the words and how many times it appears in each document)
struct WordAVLData {
    string word;	
    vector<WordStatistic> stats; 

    //compares the words and returns true if they word on thre
    bool operator<(const WordAVLData& rhs) const{ 
        return word < rhs.word; 
    }

    bool operator==(const WordAVLData& rhs) const{
        return word == rhs.word;
    }	

    bool operator==(const string& rhs) const{
        return word == rhs;
    }	

    bool operator!=(const WordAVLData& rhs) const{
        return word != rhs.word;
    }	

    bool operator!=(const string& rhs) const{
        return word != rhs;
    }	

    void operator=(const string& rhs){
        word = rhs;
    } 

    void addDoc(const string& uuid, int freq = 1){
        stats.push_back({uuid, freq});
    }

    size_t numDocs()const{
        return stats.size();
    }

    friend std::ostream& operator<<(std::ostream& out,  const WordAVLData& rhs){
        //print number of docs in the vector
        out << rhs.stats.size() << ",";
        //print the word
        out << rhs.word << ",";
        //print the vector (including uuid & word frequency)
        for(size_t i = 0; i < rhs.stats.size(); i++){
            out << rhs.stats[i].uuid << ":" << rhs.stats[i].term_freq;
            if (i != rhs.stats.size() - 1){
                out << ",";
            }
        }
        //print a newline
        out << endl;

        return out;
    }

    friend std::istream& operator>>(std::istream& in, WordAVLData& rhs){
        string uuid;
        //size of the stats vector for the word 
        size_t stats_size;
        int freq;
        //char to intake punctuation used for separating values
        char c;

        //read what the size of the vector is supposed to be
        in >> stats_size;
        in >> c;
        //read the word
        getline(in, rhs.word, ',');

        //read the uuid & freq and call addDoc 
        for (size_t i = 0; i < stats_size; i++){
            getline(in, uuid, ':');
            in >> freq;
            rhs.addDoc(uuid, freq);
            if(i != stats_size - 1){
                in >> c; 
            } 
        }
        return in;
    }															
};


#endif