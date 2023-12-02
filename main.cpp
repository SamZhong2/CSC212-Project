#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "btree.h"


int main() {
    std::string filename;
    int degree;

    // Initialize the BTree...
    std::cout << "Enter input file name: ";
    std::cin >> filename;
    std::cout << "-----------------------" << std::endl;
    
    while (true) {
        std::cout << "Enter degree or order: ";
        
        // Check if the user input is of integer type
        if (std::cin >> degree) {
            std::cout << "-----------------------" << std::endl;
            break; // Exit the loop if input is a valid integer
        } else {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter an integer." << std::endl;
            std::cout << "-----------------------" << std::endl;
        }
    }
    BTree btree(degree);
    
    

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Invvalid file" << std::endl;
        std::cout << "-----------------------" << std::endl;
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
    std::cout << "-----------------------" << std::endl;
    int count = btree.search(searchWord);
    std::cout << "Found the word \"" << searchWord << "\" a total of " << count << " times." << std::endl;
    std::cout << "-----------------------" << std::endl;

    btree.generateDotFile("btree.dot");

    return 0;
}
