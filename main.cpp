#include "btree.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Compile command:
// g++ -std=c++11 main.cpp btree.cpp -o btree

// Running the executable:
// ./btree [input_file.txt] [degree]

int main(int argc, char*argv[]) {

	// This program will add the words of an input file to the tree.

	std::ifstream input_file(argv[1]);
	BTree<std::string> word_tree(std::stoi(argv[2]));
	
	std::string line;
	std::getline(input_file, line);
	std::stringstream ss(line);

	std::string element;
	while(ss >> element){
		word_tree.insert(element);
	}

	return 0;
}