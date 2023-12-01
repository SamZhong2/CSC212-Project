#include "btree.h"

const int t = 2;

BTree::BTree(){
    this->root = nullptr;
}

void BTree::splitChild(BTreeNode* parent, int index) {
    BTreeNode* new_child = new BTreeNode(true);
    BTreeNode* old_child = parent->children[index];
    parent->keys.insert(parent->keys.begin() + index, old_child->keys[t - 1]);
    parent->counts.insert(parent->counts.begin() + index, old_child->counts[t - 1]);
    parent->children.insert(parent->children.begin() + index + 1, new_child);

    new_child->keys.assign(old_child->keys.begin() + t, old_child->keys.end());
    old_child->keys.resize(t - 1);

    new_child->counts.assign(old_child->counts.begin() + t, old_child->counts.end());
    old_child->counts.resize(t - 1);

    if (!old_child->is_leaf) {
        new_child->children.assign(old_child->children.begin() + t, old_child->children.end());
        old_child->children.resize(t);
    }
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
