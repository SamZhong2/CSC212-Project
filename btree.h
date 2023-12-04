#include <iostream>
#include <vector>
#include <string>

class BTreeNode {
private:
    bool is_leaf;
    std::vector<std::string> keys;
    std::vector<BTreeNode*> children;
    std::vector<int> count;
    friend class BTree;

public:
    BTreeNode();
    BTreeNode(bool leaf);

    int findKeyIndex(std::string& key);
};

class BTree {
private:
    BTreeNode* root;
    int degree;

    void splitChild(BTreeNode* parent, int index);
    void insertNonFull(BTreeNode* node, std::string& key);
    void dotFileHelper(BTreeNode* node, int& node_count, std::string& nodes, std::string& arrows);
    void searchHelper(BTreeNode* node, std::string& key, int& count);
    bool updateCount(BTreeNode* node, std::string& key);

public:
    BTree();
    BTree(int _degree);

    void insert(std::string& key);
    void open(std::string& key);
    int search(std::string& key);
    void generateDotFile(std::string filename);
    void remove(BTreeNode *node, std::string &key);

    void removeFromLeaf(BTreeNode *node, int index);

    void borrowLeft(BTreeNode *parent, int childIndex);

    void borrowRight(BTreeNode *parent, int childIndex);

    void mergeLeft(BTreeNode *parent, int childIndex);

    void mergeRight(BTreeNode *parent, int childIndex);

    void becomeOneWithChild(BTreeNode *parent, int childIndex);

    void removeStarter(std::string &key);
};
