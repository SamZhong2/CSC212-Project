#include <iostream>
#include <vector>
#include <string>

class BTreeNode {
    private:
        bool is_leaf;
        std::vector<std::string> keys;
        std::vector<int> counts;
        std::vector<BTreeNode*> children;
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

    public:
        BTree();
        BTree(int _degree);

        void insert(std::string& key);
        void open(std::string& key);
        int search(std::string& key);
        void generateDotFile(std::string filename);
};
