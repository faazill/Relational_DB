#pragma once
#include <string>
#include <vector>
#include <optional>
#include "kv_store.h"

using namespace std;

// This represents one finished row in our database (e.g. ID: 1, Columns: ["Alice", "25"])
struct Row {
    int id;
    vector<string> columns;
};

class Table {
public:
    // Every table needs a name and a pointer to our KV Store Delivery Truck
    Table(string name, KeyValueStore* store);

    // Insert a new row
    void insertRow(int id, vector<string> columns);
    
    // Find a row by its ID
    optional<Row> getRow(int id);

private:
    string tableName;
    KeyValueStore* kvStore; 
    
    // Helper to pack our list of strings into one single string box
    string pack(vector<string> columns);
    
    // Helper to unpack a single string box back into a list of strings
    vector<string> unpack(string data);
};
