#include "table.h"

using namespace std;

Table::Table(string name, KeyValueStore* store) {
    tableName = name;
    kvStore = store;
}

// ---------------------------------------------------------
// PACKING & UNPACKING HELPERS
// ---------------------------------------------------------

// Pack a list of strings into one string separated by "|"
// Example: ["Alice", "25"] becomes "Alice|25|"
string Table::pack(vector<string> columns) {
    string packedData = "";
    for (int i = 0; i < columns.size(); i++) {
        packedData += columns[i] + "|";
    }
    return packedData;
}

// Unpack the string back into a list
// Example: "Alice|25|" becomes ["Alice", "25"]
vector<string> Table::unpack(string data) {
    vector<string> columns;
    string currentWord = "";
    
    for (int i = 0; i < data.length(); i++) {
        if (data[i] == '|') {
            columns.push_back(currentWord);
            currentWord = ""; // Reset for the next word
        } else {
            currentWord += data[i]; // Build the word letter by letter
        }
    }
    
    return columns;
}

// ---------------------------------------------------------
// PUBLIC FUNCTIONS
// ---------------------------------------------------------

void Table::insertRow(int id, vector<string> columns) {
    // 1. Pack the user's columns into a single string (the "box")
    string box = pack(columns);
    
    // 2. Hand the box to our Key-Value Store delivery truck
    kvStore->put(id, box);
}

optional<Row> Table::getRow(int id) {
    // 1. Ask the delivery truck to find the box by ID
    optional<string> box = kvStore->get(id);
    
    // 2. If the truck didn't find anything, return nothing
    if (!box.has_value()) {
        return nullopt;
    }
    
    // 3. If we found it, unpack the box into columns
    Row row;
    row.id = id;
    row.columns = unpack(box.value());
    
    return row;
}
