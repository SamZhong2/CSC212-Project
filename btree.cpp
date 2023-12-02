#include "btree.h"
#include <fstream>

/*
    Class Constructors
*/

BTreeNode::BTreeNode() {
    is_leaf = true;
}
BTreeNode::BTreeNode(bool leaf) {
    is_leaf = leaf;
}

BTree::BTree(){
    this->root = nullptr;
    this->degree = 2;
}

BTree::BTree(int _degree){
    this->root = nullptr;
    this->degree = _degree;
}

/*
    Class Functions
*/

int BTreeNode::findKeyIndex(std::string& key) {
    return std::distance(keys.begin(), std::lower_bound(keys.begin(), keys.end(), key));
}

void BTree::splitChild(BTreeNode* parent, int index) {
    BTreeNode* new_child = new BTreeNode(true);
    BTreeNode* old_child = parent->children[index];
    parent->keys.insert(parent->keys.begin() + index, old_child->keys[degree - 1]);
    parent->counts.insert(parent->counts.begin() + index, old_child->counts[degree - 1]);
    parent->children.insert(parent->children.begin() + index + 1, new_child);

    new_child->keys.assign(old_child->keys.begin() + degree, old_child->keys.end());
    old_child->keys.resize(degree - 1);

    new_child->counts.assign(old_child->counts.begin() + degree, old_child->counts.end());
    old_child->counts.resize(degree - 1);

    if (!old_child->is_leaf) {
        new_child->children.assign(old_child->children.begin() + degree, old_child->children.end());
        old_child->children.resize(degree);
    }
}

void BTree::insertNonFull(BTreeNode* node, std::string& key) {
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
        if (node->children[i]->keys.size() == (2 * degree - 1)) {
            splitChild(node, i);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::insert(std::string& key) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys.push_back(key);
        root->counts.push_back(1);
    } else {
        if (root->keys.size() == (2 * degree - 1)) {
            BTreeNode* new_root = new BTreeNode(false);
            new_root->children.push_back(root);
            splitChild(new_root, 0);
            root = new_root;
        }
        insertNonFull(root, key);
    }
}

int BTree::search(std::string& key) {
    int count = 0;
    searchHelper(root, key, count);
    return count;
}

void BTree::searchHelper(BTreeNode* node, std::string& key, int& count) {
    // Look through each key in node.
    for (int i=0; i < node->keys.size(); i++) {
        if (key < node->keys[i]) break;
        if (key == node->keys[i]) count=count+1;
    }

    // Check if there are no children.
    if (node->is_leaf == true) return;

    // Place before the first child.
    if (key <= node->keys[0])
        searchHelper(node->children[0], key, count);
    // Place inbetween two keys in the node.
    for (int i=1; i < node->children.size()-1; i++) {
        if (key >= node->keys[i-1] && key <= node->keys[i]) {
            searchHelper(node->children[i], key, count);
        }
    }
    // place after the last child
    if (key >= node->keys[node->keys.size()-1])
        searchHelper(node->children[node->children.size()-1], key, count);

}

void BTree::open(std::string &key) {
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

void BTree::generateDotFile(std::string filename) {

    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "Error opening DOT file." << std::endl;
        return;
    }

    dotFile << "digraph BTree {\n";
    dotFile << "node [shape=record];\n";

    std::string nodes;
    std::string arrows;

    // Recursively draw each node.
    int node_count = 0;
    dotFileHelper(root, node_count, nodes, arrows);

    dotFile << nodes << arrows;

    dotFile << "}\n";
    dotFile.close();
}

void BTree::dotFileHelper(BTreeNode* node, int& node_count, std::string& nodes, std::string& arrows) {

    // Add Node to string.
    nodes = nodes + "    node" + std::to_string(node_count) + " [label=\"";
    for (int i=0; i < node->keys.size(); i++) {
        nodes = nodes + node->keys[i] + " | ";
    }
    nodes = nodes.substr(0, nodes.size() - 3);
    nodes = nodes + "\"];\n";

    // Add child nodes recursively and their children.
    int parent = node_count;
    for (int i=0; i < node->children.size(); i++) {
        node_count++;
        arrows = arrows + "    node" + std::to_string(parent) + " -> node" + std::to_string(node_count) + ";\n";
        dotFileHelper(node->children[i], node_count, nodes, arrows);
    }

}