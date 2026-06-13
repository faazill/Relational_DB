#include "kv_store.h"

using namespace std;

// Constructor: Set up the WAL and Disk with specific file names
KeyValueStore::KeyValueStore() : wal("database.wal"), disk("database.bin") {
    
    // 1. When the database turns on, we must load our data into the fast Index!
    // First, read everything from the slow hard drive (.bin file)
    vector<SavedRow> savedRows = disk.readAllRows();
    for (int i = 0; i < savedRows.size(); i++) {
        tree.insert(savedRows[i].id, savedRows[i].data);
    }
    
    // 2. Crash Recovery! 
    // What if the power went out right before we saved to the .bin file?
    // We check the WAL text file for any missing operations.
    vector<LogRecord> logs = wal.readAllLogs();
    for (int i = 0; i < logs.size(); i++) {
        if (logs[i].operation == "INSERT") {
            // Put it into the tree
            tree.insert(logs[i].rowId, logs[i].rowData);
            // Save it properly to the disk now
            disk.insertRow(logs[i].rowId, logs[i].rowData);
        } else if (logs[i].operation == "DELETE") {
            disk.deleteRow(logs[i].rowId);
        }
    }
    
    // 3. Now that everything is safely loaded and recovered, wipe the WAL clean!
    wal.clearLog();
}

KeyValueStore::~KeyValueStore() {
    // We don't need to do anything special when shutting down cleanly
}

void KeyValueStore::put(int key, string value) {
    // 1. SAFETY FIRST: Write to the WAL file immediately in case we crash
    wal.addLog("INSERT", "table", key, value);
    
    // 2. SPEED SECOND: Update our fast in-memory B+ Tree index
    tree.insert(key, value);
    
    // 3. PERSISTENCE THIRD: Write the actual binary data to the disk
    disk.insertRow(key, value);
}

optional<string> KeyValueStore::get(int key) {
    // To read data, we ONLY need to check the extremely fast B+ Tree!
    // We don't even have to touch the slow hard drive at all.
    return tree.search(key);
}
