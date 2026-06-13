#pragma once
#include <string>
#include <optional>
#include "../wal/wal_manager.h"
#include "disk_manager.h"
#include "../btree/bplus_tree.h"

using namespace std;

// This class glues our 3 pieces together into one easy-to-use database wrapper.
class KeyValueStore {
public:
    KeyValueStore();
    ~KeyValueStore();

    // The only two functions the rest of our application needs to care about!
    void put(int key, string value);
    optional<string> get(int key);

private:
    WriteAheadLog wal;
    DiskManager disk;
    IndexTree tree;
};
