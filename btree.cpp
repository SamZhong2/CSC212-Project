#include "btree.h"

const int t = 2;

BTree::BTree(){
    this->root = nullptr;
}

void BTree::splitChild(BTreeNode* parent, int index) {

}

void BTree::insertNonFull(BTreeNode* node, const std::string& key) {
    
}

void BTree::insert(const std::string& key) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys.push_back(key);
        root->counts.push_back(1);
    } else {
        if (root->keys.size() == (2 * t - 1)) {
            BTreeNode* new_root = new BTreeNode(false);
            new_root->children.push_back(root);
            splitChild(new_root, 0);
            root = new_root;
        }
    }
}

void BTree::search(const std::string& key) {

}

void BTree::generateDotFile(const std::string& filename) {

}
