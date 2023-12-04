#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

template <typename T>
class BTreeNode {
    int degree;  // Minimum degree (defines the range for number of keys)
    bool isLeaf;

public:
    BTreeNode(int _degree, bool _isLeaf);

    void traverse();

    BTreeNode<T> *search(T key);
    void insert(T key);

    void insertNonFull(T key);

    BTreeNode<T> **children;
    T *keys;

    void splitChild(int childIndex, BTreeNode<T> *child);

    int numKeys;
private:
    int findKeyIndex(T key);

};

template <typename T>
class BTree {
    BTreeNode<T> *root;
    int degree;

public:
    BTree(int _degree) : root(nullptr), degree(_degree) {}

    void traverse() {
        if (root != nullptr) {
            root->traverse();
        }
    }

    void insert(T key) {
        if (root == nullptr) {
            root = new BTreeNode<T>(degree, true);
            root->keys[0] = key;
            root->numKeys = 1;
        } else {
            if (root->numKeys == 2 * degree - 1) {
                BTreeNode<T> *s = new BTreeNode<T>(degree, false);
                s->children[0] = root;
                s->splitChild(0, root);
                int i = 0;
                if (s->keys[0] < key) {
                    i++;
                }
                s->children[i]->insertNonFull(key);
                root = s;
            } else {
                root->insertNonFull(key);
            }
        }
    }
};

template <typename T>
BTreeNode<T>::BTreeNode(int _degree, bool _isLeaf)
        : degree(_degree), isLeaf(_isLeaf), numKeys(0) {
    keys = new T[2 * degree - 1];
    children = new BTreeNode<T> *[2 * degree];
}

template <typename T>
void BTreeNode<T>::traverse() {
    int i;
    for (i = 0; i < numKeys; i++) {
        if (!isLeaf) {
            children[i]->traverse();
        }
        std::cout << " " << keys[i];
    }
    if (!isLeaf) {
        children[i]->traverse();
    }
}

template <typename T>
BTreeNode<T> *BTreeNode<T>::search(T key) {
    int i = 0;
    while (i < numKeys && key > keys[i]) {
        i++;
    }
    if (keys[i] == key) {
        return this;
    }
    if (isLeaf) {
        return nullptr;
    }
    return children[i]->search(key);
}

template <typename T>
int BTreeNode<T>::findKeyIndex(T key) {
    int index = 0;
    while (index < numKeys && keys[index] < key) {
        ++index;
    }
    return index;
}

template <typename T>
void BTreeNode<T>::splitChild(int childIndex, BTreeNode<T> *child) {
    BTreeNode<T> *newChild = new BTreeNode<T>(child->degree, child->isLeaf);
    newChild->numKeys = degree - 1;
    for (int i = 0; i < degree - 1; ++i) {
        newChild->keys[i] = child->keys[i + degree];
    }
    if (!child->isLeaf) {
        for (int i = 0; i < degree; ++i) {
            newChild->children[i] = child->children[i + degree];
        }
    }
    child->numKeys = degree - 1;
    for (int i = numKeys; i > childIndex; --i) {
        children[i + 1] = children[i];
    }
    children[childIndex + 1] = newChild;
    for (int i = numKeys - 1; i >= childIndex; --i) {
        keys[i + 1] = keys[i];
    }
    keys[childIndex] = child->keys[degree - 1];
    numKeys++;
}

template <typename T>
void BTreeNode<T>::insertNonFull(T key) {
    int i = numKeys - 1;
    if (isLeaf) {
        while (i >= 0 && key < keys[i]) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
        numKeys++;
    } else {
        while (i >= 0 && key < keys[i]) {
            i--;
        }
        if (children[i + 1]->numKeys == 2 * degree - 1) {
            splitChild(i + 1, children[i + 1]);
            if (key > keys[i + 1]) {
                i++;
            }
        }
        children[i + 1]->insertNonFull(key);
    }
}

int main() {
    std::string fileName;
    int mode;

    std::cout << "Enter your file name: " << std::endl;
    std::cin >> fileName;
    std::cout << "Enter the mode: " << std::endl;
    std::cin >> mode;
    BTree<std::string> bTree(mode);
    std::string word;

    std::ifstream ifs(fileName);
    while (ifs >> word) {
        bTree.insert(word);
    }

    bTree.traverse();

    return 0;
}
