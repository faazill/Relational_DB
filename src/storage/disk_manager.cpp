#include "disk_manager.h"
#include <fstream>
#include <map>

using namespace std;

DiskManager::DiskManager(string path) {
    filePath = path;
    
    // Quick trick to make sure the file exists before we start using it.
    // We open it in "append" mode, which creates it if it's missing.
    ofstream create_file(filePath, ios::app);
    create_file.close();
}

void DiskManager::insertRow(int id, string data) {
    // Open file in "binary" and "append" mode
    ofstream file(filePath, ios::binary | ios::app);
    
    if (file.is_open()) {
        // 1. Write the ID (4 bytes)
        file.write((char*)&id, sizeof(int));
        
        // 2. Write the length of the string (4 bytes)
        int length = data.length();
        file.write((char*)&length, sizeof(int));
        
        // 3. Write the actual text characters
        file.write(data.c_str(), length);
        
        file.close();
    }
}

optional<string> DiskManager::readRow(int id) {
    ifstream file(filePath, ios::binary);
    
    optional<string> foundData = nullopt;
    
    if (file.is_open()) {
        int currentId;
        int length;
        
        // Read the file chunk by chunk until we hit the end
        while (file.read((char*)&currentId, sizeof(int))) {
            file.read((char*)&length, sizeof(int));
            
            if (length < 0) {
                // A negative length is our secret code for "DELETED" (Tombstone)
                if (currentId == id) {
                    foundData = nullopt; // It was deleted!
                }
            } else {
                // Create a blank string of the right size, then read the text into it
                string data;
                data.resize(length);
                file.read(&data[0], length);
                
                // If this is the ID we are looking for, save it!
                // (We keep reading until the end of the file because a newer update 
                // for this row might be at the end of the file)
                if (currentId == id) {
                    foundData = data;
                }
            }
        }
        file.close();
    }
    
    return foundData;
}

vector<SavedRow> DiskManager::readAllRows() {
    ifstream file(filePath, ios::binary);
    
    // We use a Map to automatically keep only the LATEST version of each ID
    map<int, string> latestRows;
    
    if (file.is_open()) {
        int currentId;
        int length;
        
        while (file.read((char*)&currentId, sizeof(int))) {
            file.read((char*)&length, sizeof(int));
            
            if (length < 0) {
                // Deleted! Remove it from our map
                latestRows.erase(currentId);
            } else {
                string data;
                data.resize(length);
                file.read(&data[0], length);
                
                // Update or insert into our map
                latestRows[currentId] = data; 
            }
        }
        file.close();
    }
    
    // Convert our Map into a simple Vector (list) to return
    vector<SavedRow> results;
    for (auto const& pair : latestRows) {
        SavedRow row;
        row.id = pair.first;
        row.data = pair.second;
        results.push_back(row);
    }
    
    return results;
}

bool DiskManager::deleteRow(int id) {
    // First check if it even exists
    optional<string> existing = readRow(id);
    if (!existing.has_value()) {
        return false; // Nothing to delete
    }
    
    // To delete, we append a new entry with a negative length. 
    // This "Tombstone" is much faster than rewriting the whole file!
    ofstream file(filePath, ios::binary | ios::app);
    if (file.is_open()) {
        file.write((char*)&id, sizeof(int));
        
        int tombstoneLength = -1;
        file.write((char*)&tombstoneLength, sizeof(int));
        
        file.close();
        return true;
    }
    return false;
}
