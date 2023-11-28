#pragma once

#include <iostream>

class BTree {
	private:
		BTreeNode* root;
    		//Minimum
		int t;
	public:
		BTree();
		void insert(const std::string& key);
		void search(const std::string& key);
		void display();
};
