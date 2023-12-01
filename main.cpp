#include <iostream>
#include <fstream>
#include <algorithm>
#include "btree.h"

int main() {
    BTree btree;

    std::ifstream inputFile("test_1.txt");
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
    btree.search(searchWord);

    btree.generateDotFile("btree.dot");

    return 0;
}
