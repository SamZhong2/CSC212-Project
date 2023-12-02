#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "btree.h"

// Two methods of input.
// If no command line arguments, use cin.
// Inputs...   argv[1]       argv[2]
// ./btree [input_file.txt] [degree]

int main(int argc, char*argv[]) {
    std::string filename;
    int degree;

    // Get the file name...
    if (argc < 2) {
        std::cout << "Enter input file name: ";
        std::cin >> filename;
        std::cout << "-----------------------" << std::endl;
    } else {
        filename = argv[1];
    }
    
    // Get the degree of the tree...
    if (argc < 3) {
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
    } else {
        degree = std::stoi(argv[2]);
    }

    // Initialize the BTree...
    BTree btree(degree);
    
    // Open the file...
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Invalid file" << std::endl;
        std::cout << "-----------------------" << std::endl;
        return 1;
    }

    // Adds words in file to the BTree, then close...
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
    std::cout << "Found the word \"" << searchWord << "\" a total of " << count << " time(s)." << std::endl;
    std::cout << "-----------------------" << std::endl;

    btree.generateDotFile("btree.dot");

    return 0;
}
