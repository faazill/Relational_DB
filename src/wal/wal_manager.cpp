#include "wal_manager.h"
#include <fstream>  // For reading and writing files
#include <iostream> // For printing errors if we need to

using namespace std;

// Constructor: Just save the file path so we remember it
WriteAheadLog::WriteAheadLog(string path) {
    filePath = path;
}

// Add a new log to the file
void WriteAheadLog::addLog(string operation, string tableName, int rowId, string rowData) {
    // Open the file in "app" (append) mode. This means we only add to the very end of the file.
    ofstream file(filePath, ios::app);
    
    if (file.is_open()) {
        // Write the data separated by spaces, like: "INSERT users 1 Alice"
        file << operation << " " << tableName << " " << rowId << " " << rowData << endl;
        
        // flush() forces the computer to save the data right now, making it safer
        file.flush(); 
        
        file.close();
    }
}

// Read the log file from top to bottom
vector<LogRecord> WriteAheadLog::readAllLogs() {
    vector<LogRecord> allLogs;
    
    // Open the file for reading
    ifstream file(filePath);
    
    if (file.is_open()) {
        LogRecord record;
        
        // Read word by word from the file. 
        // We know our format is: Operation TableName RowId RowData
        // This loop keeps going until we run out of words.
        while (file >> record.operation >> record.tableName >> record.rowId >> record.rowData) {
            
            // Add this record to our list
            allLogs.push_back(record);
        }
        
        file.close();
    }
    
    return allLogs;
}

// Wipe the log file clean
void WriteAheadLog::clearLog() {
    // Open the file in "trunc" (truncate) mode. This deletes all existing content immediately.
    ofstream file(filePath, ios::trunc);
    
    if (file.is_open()) {
        file.close();
    }
}