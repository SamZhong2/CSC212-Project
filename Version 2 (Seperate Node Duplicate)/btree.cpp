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
    BTreeNode* new_child = new BTreeNode();
    BTreeNode* old_child = parent->children[index];
    parent->keys.insert(parent->keys.begin() + index, old_child->keys[degree - 1]);
    parent->children.insert(parent->children.begin() + index + 1, new_child);

    new_child->keys.assign(old_child->keys.begin() + degree, old_child->keys.end());
    old_child->keys.resize(degree - 1);

    if (!old_child->is_leaf) {
        new_child->children.assign(old_child->children.begin() + degree, old_child->children.end());
        old_child->children.resize(degree);
    }

    parent->is_leaf = false;
    if (new_child->children.size()) new_child->is_leaf = false;

}

void BTree::insertNonFull(BTreeNode* node, std::string& key) {
    // Start at the end of the node...
    int i = node->keys.size() - 1;
    // Go through each key until you find a place to put the new one...
    while (i >= 0 && key < node->keys[i]) {
        i--;
    }
    i++;

    // If the node is a leaf...
    if (node->is_leaf) {
        // Add the key to the node.
        node->keys.insert(node->keys.begin() + i, key);

        // The node is not a leaf...
    } else {
        if (node->children[i]->keys.size() == (2 * degree - 1)) {
            splitChild(node, i);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::insert(std::string& key) {
    // If there is no root...
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys.push_back(key);
        return;
    }
    // If the root node is reaches max...
    if (root->keys.size() == (2 * degree - 1)) {
        BTreeNode* new_root = new BTreeNode();
        new_root->children.push_back(root);
        splitChild(new_root, 0);
        root = new_root;
    }
    // Recursively insert the function...
    insertNonFull(root, key);
}

void BTree::removeStarter(std::string& key) {

    // Run the actual remove function
    remove(root, key);
    // Check if the root is out of keys, then set the root to the child.
    if (root->keys.empty() && !root->is_leaf) {
        BTreeNode* newRoot = root->children[0];
        delete root;
        root = newRoot;
    }
}

void BTree::remove(BTreeNode* node, std::string& key) {
    // set the index.
    int index = node->findKeyIndex(key);

    // The key exists in this node AND the current node is a Leaf.
    if (index < node->keys.size() && key == node->keys[index]) {
        removeFromLeaf(node, index);
    } else {
        // The Key was not found in this node; continue...

        // Check if the current node is root or not a leaf then go to the correct child node.
        if (!node->is_leaf) {
            bool lastChild = (index == node->keys.size());
            BTreeNode* child = node->children[index];

            // The child does not have the minimum keys.  Merge with a sibling to resolve.
            if (child->keys.size() < degree) {
                becomeOneWithChild(node, index);
            }

            // The child was merged, go to the child.
            if (lastChild && index > node->keys.size()) {
                index--;
            }

            // Remove the child.
            remove(node->children[index], key);
        } else {
            // Not found
            //std::cout<<"NOTHING FOUND<<std::endl;
        }
    }
}

void BTree::removeFromLeaf(BTreeNode* node, int index) {
    //deletes the keys to remove from the leaf.
    node->keys.erase(node->keys.begin() + index);
}

void BTree::borrowRight(BTreeNode* parent, int childIndex) {
    BTreeNode* child = parent->children[childIndex];
    BTreeNode* rightSibling = parent->children[childIndex + 1];

    // Parent obtains key from right child sibling.
    child->keys.push_back(parent->keys[childIndex]);
    parent->keys[childIndex] = rightSibling->keys.front();
    rightSibling->keys.erase(rightSibling->keys.begin());

    // Child is not leaf, take the pointer away from the right child sibling.
    if (!child->is_leaf) {
        child->children.push_back(rightSibling->children.front());
        rightSibling->children.erase(rightSibling->children.begin());
    }
}

void BTree::mergeRight(BTreeNode* parent, int childIndex) {
    BTreeNode *child = parent->children[childIndex];
    BTreeNode *rightSibling = parent->children[childIndex + 1];

    // Key moves from the child back to the parent.
    child->keys.push_back(parent->keys[childIndex]);

    // All keys and pointers move to the child.
    child->keys.insert(child->keys.end(), rightSibling->keys.begin(), rightSibling->keys.end());
    child->children.insert(child->children.end(), rightSibling->children.begin(), rightSibling->children.end());

    // Delete the key and child pointer.
    parent->keys.erase(parent->keys.begin() + childIndex);
    parent->children.erase(parent->children.begin() + childIndex + 1);

    // delete the child right sibling
    delete rightSibling;
}

void BTree::borrowLeft(BTreeNode* parent, int childIndex) {
    BTreeNode* child = parent->children[childIndex];
    BTreeNode* leftSibling = parent->children[childIndex - 1];

    // Parent obtains key from left child sibling.
    child->keys.insert(child->keys.begin(), parent->keys[childIndex - 1]);
    parent->keys[childIndex - 1] = leftSibling->keys.back();
    leftSibling->keys.pop_back();

    // Child is not leaf, take the pointer away from the left child sibling.
    if (!child->is_leaf) {
        child->children.insert(child->children.begin(), leftSibling->children.back());
        leftSibling->children.pop_back();
    }
}

void BTree::mergeLeft(BTreeNode* parent, int childIndex) {
    BTreeNode *child = parent->children[childIndex];
    BTreeNode *leftSibling = parent->children[childIndex - 1];

    // key moves from parent to left sibling.
    leftSibling->keys.push_back(parent->keys[childIndex - 1]);

    // All keys and points are shifted to the left.
    leftSibling->keys.insert(leftSibling->keys.end(), child->keys.begin(), child->keys.end());
    leftSibling->children.insert(leftSibling->children.end(), child->children.begin(), child->children.end());

    // Delete the key and child pointer from the parent.
    parent->keys.erase(parent->keys.begin() + childIndex - 1);
    parent->children.erase(parent->children.begin() + childIndex);

    // delete the child.
    delete child;
}
void BTree::becomeOneWithChild(BTreeNode* parent, int childIndex) {
    BTreeNode* sibling;
    if (childIndex > 0) {
        sibling = parent->children[childIndex - 1];
    } else {
        sibling = nullptr;
    }

    // Take from left.
    if (degree <= sibling->keys.size() && sibling) {
        borrowLeft(parent, childIndex);
    } else if (childIndex < parent->keys.size() && parent->children[childIndex + 1]->keys.size() >= degree) { // take from the right

        borrowRight(parent, childIndex);
    } else if (sibling) { // combine left.
        mergeLeft(parent, childIndex);
    } else { // combine right.
        mergeRight(parent, childIndex);
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

/*void BTree::open(std::string &key) {
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
}*/

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
    nodes = nodes + "\"";
    if (node->is_leaf) nodes = nodes + " color=\"#62b362\"";
    nodes = nodes + "];\n";

    // Add child nodes recursively and their children.
    int parent = node_count;
    for (int i=0; i < node->children.size(); i++) {
        node_count++;
        arrows = arrows + "    node" + std::to_string(parent) + " -> node" + std::to_string(node_count) + ";\n";
        dotFileHelper(node->children[i], node_count, nodes, arrows);
    }

}