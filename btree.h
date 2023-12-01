#pragma once
#include <iostream>
#include <vector>
#include <string>

class BTreeNode {
public:
    bool is_leaf;
    std::vector<std::string> keys;
    std::vector<int> counts;
    std::vector<BTreeNode*> children;

    BTreeNode(bool leaf = true) {
        this->is_leaf = leaf;
    }

    int findKeyIndex(const std::string& key) {
        return std::distance(keys.begin(), std::lower_bound(keys.begin(), keys.end(), key));
    }
};

class BTree {
private:
    BTreeNode* root;

    void splitChild(BTreeNode* parent, int index);
    void insertNonFull(BTreeNode* node, const std::string& key);

public:
    BTree();

    void insert(const std::string& key);
    void search(const std::string& key);
    void generateDotFile(const std::string& filename);
};
