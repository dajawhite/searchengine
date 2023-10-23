#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include <iostream>
#include <algorithm>
#include "DocParser.h"
#include "WordSanitizer.h"
#include "QueryProcessor.h"

/*TEST_CASE("Check Read & Write", "[WordAVLData]"){
    WordAVLData test, user;
    string name;
    test.word = "names";
    test.addDoc("6289178082787gvshU8Y88ADUCV8FS", 3);
    test.addDoc("jdewi797CXSHBC7", 10);
    test.addDoc("NKJDWSLE728BHELW82", 50);
    test.addDoc("BDS7HUCD9D0Y06GBSABK9Hk", 1000);

    cout << test << endl;
    cin >> user;
    cout << endl << user.word << " ";
    cout << user.numDocs();
}

TEST_CASE("Check AVLTree", "[AvlTree]"){
    AvlTree <WordAVLData> apples;
    string species[] = {"gala", "fiji", "granny", "smith", "honeycrisp"};
    WordAVLData types[5];
    for (size_t i = 0; i < 5; i++){
        types[i] = species[i];
        types[i].addDoc("e17252151a2d36344b7104d14805414a55042abe", 3);
        types[i].addDoc("5293252386bb424828d17b0ac60299cb52a8ecb9", 100);
        types[i].addDoc("dc13b6c13ee2282e2b78a7edbcbba48fe173ea3e", 10);
        apples.insert(types[i]);
    }
    apples.remove(types[1]);
    types[1].addDoc("dc13b6c13ee2282e2b78a7edbcbba48fe173ea3a");

    apples.insert(types[1]);
    cout << apples[types[1]] << endl;
    apples.remove(types[3]);
    types[3] = "pink";
    apples.insert(types[3]);
    WordAVLData smith;
    smith.word = "smith";

    SECTION("Methods"){
        REQUIRE((apples.isEmpty() == false));
        REQUIRE((apples[types[1]].numDocs() == 4));
        REQUIRE((apples[types[3]].numDocs() == 3));
        REQUIRE(((apples[types[3]] != "smith") && (apples[types[3]] == "pink")));
        REQUIRE((apples[types[2]] != apples[types[4]]));
        REQUIRE((!(apples.contains(smith))));
    }

    apples.printPrettyTree();
}*/

TEST_CASE("Check that DocParser works", "[DocParser]")
{
    IndexHandler index_handler;
    DocParser doc_parser(&index_handler);    //DocParser default constructor is called

    SECTION("Testing process_folder()") {

        doc_parser.process_folder("sample_data");

        string search_word = "stocks";
        WordSanitizer::sanitize(search_word);
        auto results = index_handler.lookup_term(search_word);

        REQUIRE((!results.empty()));
        REQUIRE((results.size() == 1));
        REQUIRE((results[0].uuid == "dc13b6c13ee2282e2b78a7edbcbba48fe173ea3e"));
        REQUIRE((results[0].term_freq == 1));

        search_word = "electric";
        WordSanitizer::sanitize(search_word);
        results = index_handler.lookup_term(search_word);

        REQUIRE((results.empty()));

        search_word = "Reuters";
        WordSanitizer::sanitize(search_word);
        results = index_handler.lookup_org(search_word);

        REQUIRE((!results.empty()));
        REQUIRE((results.size() == 4));
        REQUIRE((results[0].uuid == "0c76d9defc2af9b9fc2659a9ca3d77b546850438"));
        REQUIRE((results[0].term_freq == 2));
        REQUIRE((results[2].uuid == "5293252386bb424828d17b0ac60299cb52a8ecb9"));
        REQUIRE((results[2].term_freq == 1));
        REQUIRE((results[1].uuid == "d2c20cd187389fdc2875ee2e51127aa6bfed50c6"));
        REQUIRE((results[1].term_freq == 1));
        REQUIRE((results[3].uuid == "e17252151a2d36344b7104d14805414a55042abe"));
        REQUIRE((results[3].term_freq == 1));

        search_word = "Vance";
        WordSanitizer::sanitize(search_word);
        results = index_handler.lookup_person(search_word);

        REQUIRE((!results.empty()));
        REQUIRE((results.size() == 1));
        REQUIRE((results[0].uuid == "4ad75456ae0bc42beeee2ff65fa58fa1cc503be2"));
        REQUIRE((results[0].term_freq == 1));
    }
}

TEST_CASE("Check that QueryProcessor works", "[QueryProcessor]")
{
    IndexHandler index;
    DocParser parser(&index);
    parser.process_folder("sample_data");
    QueryProcessor query(&index);

    vector<WordStatistic> results; 

    

    SECTION("Test query get_results"){
        results = query.get_results("distribution");
        REQUIRE((!results.empty()));

        results = query.get_results("market percent");
        REQUIRE((!results.empty()));
        REQUIRE((results.size() == 2));
    }


}
 //you can run the test using Run CTest in the task bar or by running the tests executable-> 
