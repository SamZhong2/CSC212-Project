#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <limits>
#include "btree.h"

// Search for a word in the tree as well as the count.
void searchTree(BTree btree){
    std::string searchWord;
    std::cout << "Enter a word to search: ";
    std::cin >> searchWord;
    std::cout << "-----------------------" << std::endl;
    int count = btree.search(searchWord);
    std::cout << "Found the word \"" << searchWord << "\" a total of " << count << " time(s)." << std::endl;
    std::cout << "-----------------------" << std::endl;
}

// Generate the Dot File.
void genDot(BTree btree){
    btree.generateDotFile("btree.dot");
    std::cout << ".DOT File Generated." << std::endl;
}

// Insert more words into the tree.
void insertPortal(BTree btree){
    std::cout << "Welcome to the insert portal."<<std::endl;
    std::cout << "When finished enter 'Done'"<<std::endl;
    std::string userInsert;
    while(userInsert != "Done"){
        std::cout<<"Enter :"; std::cin>>userInsert;
        if(userInsert=="Done"){ break; }
        btree.insert(userInsert);
        std::cout<<userInsert<<" Successfully inserted."<<std::endl;
    }
}

void removePortal(BTree btree){
    std::cout << "Welcome to the remove portal."<<std::endl;
    std::cout << "When finished enter 'Done'"<<std::endl;
    std::string userRemove;
    while(userRemove != "Done"){
        std::cout<<"Enter :"; std::cin>>userRemove;
        if(userRemove=="Done"){ break; }
        if(btree.search(userRemove)==0){
            std::cout<<userRemove<<" Does Not Exist Within the B-Tree.  Try Again"<<std::endl;
        }
        else{
            btree.removeStarter(userRemove);
            std::cout<<userRemove<<" Successfully removed."<<std::endl;
        }
    }
}

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

    // Runs the User Menu Interface.
    int input=9;
    while(input != 0){
        std::cout<< "Hello!  Welcome to the B-Tree program."<<std::endl;
        std::cout<< "What would you like to do?"<<std::endl;
        std::cout<< "(Enter the number of the action would like to perform)"<<std::endl;
        std::cout<< "(1) Search for a word."<<std::endl;
        std::cout<< "(2) Insert a word."<<std::endl;
        std::cout<< "(3) Delete a word. (Beta/Unstable) "<<std::endl;
        std::cout<< "(4) Generate a .DOT file"<<std::endl;
        std::cout<< "(0) Close the program"<<std::endl;
        std::cin>>input;
        if(input == 1){
            searchTree(btree);
        }else if(input ==2 ){
            insertPortal(btree);
        }else if (input == 3){
            removePortal(btree);
        }else if (input == 4){
            genDot(btree);
        }else if (input == 0){} else{
            std::cout<<"Incorrect Input, try again."<<std::endl;
        }
    }
    return 0;
}
