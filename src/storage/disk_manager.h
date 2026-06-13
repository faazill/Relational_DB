#pragma once
#include <string>
#include <vector>
#include <optional> // This lets us return "nothing" if a row isn't found

using namespace std;

// This represents a single row saved on our hard drive
struct SavedRow {
    int id;
    string data;
};

class DiskManager {
public:
    // Open or create the main database file
    DiskManager(string filePath);
    
    // Save a new row to the hard drive
    void insertRow(int id, string data);
    
    // Find a row by its ID
    optional<string> readRow(int id);
    
    // Load every single row into RAM (useful when the database starts up)
    vector<SavedRow> readAllRows();
    
    // Delete a row (we do this by marking it as a "tombstone")
    bool deleteRow(int id);

private:
    string filePath;
};
