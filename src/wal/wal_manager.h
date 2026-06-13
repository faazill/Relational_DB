#pragma once
#include <string>
#include <vector>

using namespace std;

// This represents one action that happened in our database
struct LogRecord {
    string operation; // example: "INSERT" or "DELETE"
    string tableName; // example: "users"
    int rowId;        // example: 1
    string rowData;   // example: "Alice"
};

class WriteAheadLog {
public:
    // When we start, we need to know where the text file is saved
    WriteAheadLog(string path);
    
    // Add a new line to our text file
    void addLog(string operation, string tableName, int rowId, string rowData);
    
    // Read all the lines from our text file
    vector<LogRecord> readAllLogs();
    
    // Delete everything in the text file
    void clearLog();

private:
    string filePath;
};
