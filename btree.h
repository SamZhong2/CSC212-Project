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
        int t;
	public:
		BTree();
		~BTree();
		void insert(const T& key);
		void search(const T& key);
		void display();
};