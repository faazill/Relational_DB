#pragma once
#include <vector>
#include <string>
#include <optional>

using namespace std;

// The maximum number of branches a node can have before it splits
const int MAX_CHILDREN = 4; 

// A single block (Node) inside our Tree
struct TreeNode {
    bool isLeaf; // True if this is at the bottom of the tree
    
    vector<int> keys;           // The IDs we are searching for
    vector<TreeNode*> children; // Branches to go down (if it's an internal node)
    vector<string> values;      // The actual data (only used if it's a leaf node)
    TreeNode* nextLeaf;         // Link to the next leaf for fast scanning

    // Constructor to easily create a new node
    TreeNode(bool leaf) {
        isLeaf = leaf;
        nextLeaf = nullptr;
    }
};

class IndexTree {
public:
    IndexTree();
    ~IndexTree();

    // Add a new ID and value into the tree
    void insert(int key, string value);
    
    // Find a value using its ID
    optional<string> search(int key);

private:
    TreeNode* root; // The very top of the tree

    // Helper functions for the complicated splitting math
    void insertIntoParent(int key, TreeNode* oldNode, TreeNode* newNode);
    TreeNode* findParent(TreeNode* cursor, TreeNode* child);
    void freeNode(TreeNode* node);
};
