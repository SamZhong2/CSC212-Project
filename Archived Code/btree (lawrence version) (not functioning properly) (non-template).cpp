#include "btree.h"
#include <fstream>
#include <algorithm>

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

void BTree::splitChild(BTreeNode* x, int i) {
    BTreeNode* y = x->children[i];
    BTreeNode* z = new BTreeNode(y->is_leaf);
    z->degree = this->degree;

    for (int j = 0; j < this->degree - 1; j++) {
        z->keys.push_back(y->keys[j + this->degree]);
    }

    if (!y->is_leaf) {
        for (int j = 0; j < this->degree; j++) {
            z->children.push_back(y->children[j + this->degree]);
        }
    }

    y->keys.resize(this->degree - 1);
    y->children.resize(this->degree);

    x->children.insert(x->children.begin() + i + 1, z);
    x->keys.insert(x->keys.begin() + i, y->keys[this->degree - 1]);
    x->degree = x->degree + 1;
}
void BTree::insert(std::string &key) {
    if (this->root == nullptr) {
        this->root = new BTreeNode(this->degree);
        this->root->keys.push_back(key);
    } else {
        if (this->root->keys.size() == (2 * this->degree) - 1) {
            BTreeNode* oldRoot = this->root;
            this->root = new BTreeNode(this->degree);
            this->root->children.push_back(oldRoot);
            splitChild(this->root, 0);
            insertNonFull(this->root, key);
        } else {
            insertNonFull(this->root, key);
        }
    }
}

void BTree::insertNonFull(BTreeNode* x, std::string &key) {
    int i = x->keys.size() - 1;

    if (x->is_leaf) {
        x->keys.insert(x->keys.begin() + i + 1, key);
        sort(x->keys.begin(), x->keys.end());
    } else {
        while (i >= 0 && key < x->keys[i]) {
            i--;
        }
        i++;
        if (x->children[i]->keys.size() == (2 * this->degree) - 1) {
            splitChild(x, i);
            if (key > x->keys[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], key);
    }
}


void BTree::remove(std::string& key) {

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
    if (key < node->keys[0]) // changed from key <= node->keys[0]
        searchHelper(node->children[0], key, count);
    // Place inbetween two keys in the node.
    for (int i=1; i < node->children.size(); i++) {
        if (key >= node->keys[i-1] && key < node->keys[i]) { // changed from key <= node->keys[i]
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
    nodes = nodes + "   node" + std::to_string(node_count) + " [label=\"";
    for (int i=0; i < node->keys.size(); i++) {
        nodes = nodes + node->keys[i] + " | ";
    }
    nodes = nodes.substr(0, nodes.size() - 3);
    nodes = nodes + "\"];\n";

    // Add child nodes recursively and their children.
    int parent = node_count;
    for (int i=0; i < node->children.size(); i++) {
        if(node->children[i]) { // check if the child exists
            node_count++;
            arrows = arrows + "   node" + std::to_string(parent) + " -> node" + std::to_string(node_count) + ";\n";
            dotFileHelper(node->children[i], node_count, nodes, arrows);
        }
    }
}
