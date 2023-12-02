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

        int findKeyIndex(const std::string& key);
};

class BTree {
    private:
        BTreeNode* root;
        int degree;

        void splitChild(BTreeNode* parent, int index);
        void insertNonFull(BTreeNode* node, const std::string& key);
        void dotFileHelper(BTreeNode* node, std::string& nodes, std::string& arrows);

    public:
        BTree();
        BTree(int _degree);

        void insert(const std::string& key);
        void open(const std::string& key);
        void search(const std::string& key);
        void generateDotFile(const std::string& filename);
};
