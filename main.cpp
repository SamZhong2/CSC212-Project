#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "btree.h"

// Inputs...       2           3
// ./btree [input_file.txt] [degree]

int main(int argc, char*argv[]) {

    // Initialize the BTree...
    BTree btree;
    if (argc > 2) {
        btree = BTree(std::stoi(argv[2]));
    }
    // Get the filename...
    std::string filename = "test_1.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    std::string word;
    while (inputFile >> word) {
        btree.insert(word);
    }

    inputFile.close();

    std::string searchWord;
    std::cout << "Enter a word to search: ";
    std::cin >> searchWord;
    int count = btree.search(searchWord);
    std::cout << "Found the word \"" << searchWord << "\" a total of " << count << " times." << std::endl;

    btree.generateDotFile("btree.dot");

    return 0;
}
