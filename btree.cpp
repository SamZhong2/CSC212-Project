#include "btree.h"

const int t = 2;

BTree::BTree(){
    this->root = nullptr;
}

void BTree::splitChild(BTreeNode* parent, int index) {

}

void BTree::insertNonFull(BTreeNode* node, const std::string& key) {
    int i = node->keys.size() - 1;

    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        node->keys.insert(node->keys.begin() + i + 1, key);
        node->counts.insert(node->counts.begin() + i + 1, 1);
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }

        i++;
        if (node->children[i]->keys.size() == (2 * t - 1)) {
            splitChild(node, i);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
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
        insertNonFull(root, key);
    }
}

void BTree::search(const std::string& key) {

}

void BTree::generateDotFile(const std::string& filename) {

}
