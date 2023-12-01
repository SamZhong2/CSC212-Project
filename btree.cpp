#include "btree.h"
#include <fstream>

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
    BTreeNode* current = root;
    int count = 0;

    while (current) {
        int i = current->findKeyIndex(key);

        if (i < current->keys.size() && key == current->keys[i]) {
            count += current->counts[i];
            break;
        }

        if (current->is_leaf) {
            break;
        }

        current = current->children[i];
    }

    if (count > 0) {
        std::cout << "Word found with count: " << count << std::endl;
    } else {
        std::cout << "Word not found" << std::endl;
    }
}

void BTree::open(const std::string &key) {
    BTreeNode* current = root;
    int count = 0;

    while (current) {
        int i = current->findKeyIndex(key);

        if (i < current->keys.size() && key == current->keys[i]) {
            count += current->counts[i];
            break;
        }

        if (current->is_leaf) {
            break;
        }

        current = current->children[i];
    }

    if (count > 0) {
        std::cout << "-------------------------" << std::endl;
        std::cout << "File Successfully Opened " << std::endl;
        std::cout << "-------------------------" << std::endl;
    } else {
        std::cout << "-------------------------" << std::endl;
        std::cout << "File not found" << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
}

void BTree::generateDotFile(const std::string& filename) {
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "Error opening DOT file." << std::endl;
        return;
    }

    dotFile << "digraph BTree {\n";
    dotFile << "node [shape=record];\n";

    std::vector<BTreeNode*> nodes;
    std::vector<std::string> labels;

    nodes.push_back(root);
    labels.push_back("");

    for (int i = 0; i < nodes.size(); i++) {
        BTreeNode* current = nodes[i];
        dotFile << "node" << i << " [label=\"";
        for (int j = 0; j < current->keys.size(); j++) {
            dotFile << "<f" << j << "> " << current->keys[j];
            if (j < current->keys.size() - 1) {
                dotFile << " | ";
            }
        }
        dotFile << "\"];\n";

        for (int j = 0; j < current->children.size(); j++) {
            nodes.push_back(current->children[j]);
            labels.push_back("f" + std::to_string(j));
        }
    }

    for (int i = 0; i < nodes.size(); i++) {
        BTreeNode* current = nodes[i];
        for (int j = 0; j < current->children.size(); j++) {
            dotFile << "node" << i << ":f" << j << " -> node" << labels[i + 1 + j] << ";\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();
}
