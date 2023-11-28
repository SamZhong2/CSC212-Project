#pragma once

#include <iostream>
#include <vector>
#include <string>

struct BTreeNode {
    std::vector<std::string> keys;
    std::vector<BTreeNode*> children;
    bool isLeaf;

    BTreeNode(bool _isLeaf);

    // Function to traverse the tree
    void traverse();
};

class BTree {
	private:
        BTreeNode* root;
        int t;
	public:
		BTree();
		void insert(const std::string& key);
		void search(const std::string& key);
		void display();
};
