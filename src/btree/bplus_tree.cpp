#include "bplus_tree.h"

using namespace std;

IndexTree::IndexTree() {
    // We start with one empty leaf node at the top
    root = new TreeNode(true);
}

IndexTree::~IndexTree() {
    freeNode(root);
}

void IndexTree::freeNode(TreeNode* node) {
    if (node == nullptr) return;
    if (!node->isLeaf) {
        for (int i = 0; i < node->children.size(); i++) {
            freeNode(node->children[i]);
        }
    }
    delete node;
}

optional<string> IndexTree::search(int key) {
    if (root == nullptr) return nullopt;
    
    TreeNode* cursor = root;
    
    // 1. Travel down the tree branches until we hit a leaf at the bottom
    while (!cursor->isLeaf) {
        int i = 0;
        while (i < cursor->keys.size() && key >= cursor->keys[i]) {
            i++;
        }
        cursor = cursor->children[i];
    }
    
    // 2. We are at the leaf! Search for our key.
    for (int i = 0; i < cursor->keys.size(); i++) {
        if (cursor->keys[i] == key) {
            return cursor->values[i];
        }
    }
    
    return nullopt; // Not found
}

void IndexTree::insert(int key, string value) {
    if (root == nullptr) {
        root = new TreeNode(true);
        root->keys.push_back(key);
        root->values.push_back(value);
        return;
    }

    TreeNode* cursor = root;
    TreeNode* parent = nullptr;

    // 1. Travel down to find the correct leaf to insert into
    while (!cursor->isLeaf) {
        parent = cursor;
        int i = 0;
        while (i < cursor->keys.size() && key >= cursor->keys[i]) {
            i++;
        }
        cursor = cursor->children[i];
    }

    // 2. Insert into the leaf in sorted order
    int i = 0;
    while (i < cursor->keys.size() && key > cursor->keys[i]) {
        i++;
    }
    
    // If it already exists, just update the value
    if (i < cursor->keys.size() && cursor->keys[i] == key) {
        cursor->values[i] = value;
        return;
    }

    cursor->keys.insert(cursor->keys.begin() + i, key);
    cursor->values.insert(cursor->values.begin() + i, value);

    // 3. If the node is too full, we must SPLIT it! (This is the complex part)
    if (cursor->keys.size() == MAX_CHILDREN) {
        TreeNode* newLeaf = new TreeNode(true);
        int splitIndex = MAX_CHILDREN / 2;
        
        // Move half the data to the new leaf
        for (int j = splitIndex; j < cursor->keys.size(); j++) {
            newLeaf->keys.push_back(cursor->keys[j]);
            newLeaf->values.push_back(cursor->values[j]);
        }
        
        // Shrink the old leaf
        cursor->keys.resize(splitIndex);
        cursor->values.resize(splitIndex);
        
        // Fix the linked list pointers
        newLeaf->nextLeaf = cursor->nextLeaf;
        cursor->nextLeaf = newLeaf;

        // If we split the very top root, create a new root above them
        if (cursor == root) {
            TreeNode* newRoot = new TreeNode(false);
            newRoot->keys.push_back(newLeaf->keys[0]);
            newRoot->children.push_back(cursor);
            newRoot->children.push_back(newLeaf);
            root = newRoot;
        } else {
            // Otherwise, push the new split up to the parent branch
            insertIntoParent(newLeaf->keys[0], parent, newLeaf);
        }
    }
}

// Helper to push a split up the tree
void IndexTree::insertIntoParent(int key, TreeNode* cursor, TreeNode* child) {
    int i = 0;
    while (i < cursor->keys.size() && key > cursor->keys[i]) i++;
    
    cursor->keys.insert(cursor->keys.begin() + i, key);
    cursor->children.insert(cursor->children.begin() + i + 1, child);

    if (cursor->keys.size() == MAX_CHILDREN) {
        TreeNode* newInternal = new TreeNode(false);
        int split = MAX_CHILDREN / 2;
        int upKey = cursor->keys[split];

        for (int j = split + 1; j < cursor->keys.size(); j++) {
            newInternal->keys.push_back(cursor->keys[j]);
        }
        for (int j = split + 1; j < cursor->children.size(); j++) {
            newInternal->children.push_back(cursor->children[j]);
        }
        
        cursor->keys.resize(split);
        cursor->children.resize(split + 1);

        if (cursor == root) {
            TreeNode* newRoot = new TreeNode(false);
            newRoot->keys.push_back(upKey);
            newRoot->children.push_back(cursor);
            newRoot->children.push_back(newInternal);
            root = newRoot;
        } else {
            insertIntoParent(upKey, findParent(root, cursor), newInternal);
        }
    }
}

// Helper to find a parent node
TreeNode* IndexTree::findParent(TreeNode* cursor, TreeNode* child) {
    if (cursor->isLeaf || cursor->children.empty()) return nullptr;
    
    for (int i = 0; i < cursor->children.size(); i++) {
        if (cursor->children[i] == child) {
            return cursor;
        } else {
            TreeNode* parent = findParent(cursor->children[i], child);
            if (parent != nullptr) return parent;
        }
    }
    return nullptr;
}
