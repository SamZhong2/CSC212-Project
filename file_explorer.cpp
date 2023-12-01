#include <iostream>
#include <algorithm>
#include "btree.h"

int main() {
    BTree btree;
    std::string entry;

    std::cout << "-------------------------" << std::endl;
    std::cout << "Type 'end' when finished" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Enter file name: ";
    while (std::cin >> entry) {
        if (entry == "end") {
            break;
        }
        btree.insert(entry);
        std::cout << "- '" << entry << "' added" << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "Enter file name: ";
    }

    std::string searchWord;
    std::cout << "Enter a file to open: ";
    std::cin >> searchWord;
    btree.open(searchWord);

    btree.generateDotFile("btree.dot");

    return 0;
}