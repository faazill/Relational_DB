#include <iostream>
#include <optional>
#include "../src/btree/bplus_tree.h"

using namespace std;

int main() {
    cout << "--- Testing the B+ Tree Index ---" << endl;

    IndexTree tree;

    // 1. Let's insert IDs in random order to test sorting
    cout << "Inserting IDs: 50, 20, 80, 10, 30..." << endl;
    tree.insert(50, "Fifty");
    tree.insert(20, "Twenty");
    tree.insert(80, "Eighty");
    tree.insert(10, "Ten");
    tree.insert(30, "Thirty");

    // 2. Let's force the tree to SPLIT by adding a bunch of numbers!
    // Since MAX_CHILDREN is 4, adding 10 items will definitely cause splits.
    for (int i = 100; i < 110; i++) {
        tree.insert(i, "Data-" + to_string(i));
    }
    cout << "Inserted more data to force a split!" << endl;

    // 3. Now let's try to find an ID. It should search the branches lightning fast.
    optional<string> result1 = tree.search(20);
    if (result1.has_value()) {
        cout << "Found ID 20: " << result1.value() << endl;
    } else {
        cout << "Failed to find ID 20!" << endl;
    }

    optional<string> result2 = tree.search(105);
    if (result2.has_value()) {
        cout << "Found ID 105: " << result2.value() << endl;
    } else {
        cout << "Failed to find ID 105!" << endl;
    }

    // 4. Try to search for something that doesn't exist
    optional<string> result3 = tree.search(999);
    if (!result3.has_value()) {
        cout << "Correctly failed to find ID 999 (it doesn't exist)." << endl;
    }

    return 0;
}
