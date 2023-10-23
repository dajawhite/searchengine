#include <iostream>
#include <string>
#include "DocParser.h"

using namespace std;


int main (){
    IndexHandler index_handler;
	DocParser doc_parser(&index_handler);
	doc_parser.process_folder("sample_data");
    
    return 0;
}
