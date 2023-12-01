#include <iostream>
#include <string>
#include "btree.h"

// Compile command:
// g++ -std=c++11 main.cpp btree.cpp -o btree

int main(int argc, char*argv[]) {

	BTree<std::string> my_tree();
	std::cout << "working..." << std::endl;

	return 0;
}