#pragma once
#include <iostream>
#include <vector>
#include <string>

template <typename T>
struct Node {
	private:
		std::vector<T> keys;
		std::vector<Node*> children;
		bool isLeaf;
	public:
    	Node(bool _isLeaf);
		~Node();

    	// Function to traverse the tree
    	void traverse();
};

template <typename T>
class BTree {
	private:
        Node<T>* root;
        int t; // t being the degree.
	public:
		BTree();
		BTree(int _t);
		~BTree();
		
		void insert(T key);
		void search(T key);
		void remove(T key);
		void display();
};