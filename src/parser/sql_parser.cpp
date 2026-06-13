#include "sql_parser.h"
#include <sstream>

using namespace std;

SQLParser::SQLParser(string query) {
    rawQuery = query;
}

// Tokenize just splits the sentence by spaces into words.
// E.g. "INSERT users 1 Alice" -> ["INSERT", "users", "1", "Alice"]
vector<string> SQLParser::tokenize() {
    vector<string> words;
    stringstream ss(rawQuery); // stringstream is a great C++ tool for splitting by spaces!
    string word;
    
    while (ss >> word) {
        words.push_back(word);
    }
    
    return words;
}

ParsedCommand SQLParser::parse() {
    ParsedCommand command;
    command.type = CommandType::UNKNOWN;
    
    vector<string> words = tokenize();
    
    // If they just hit Enter without typing anything, return early
    if (words.size() == 0) return command;

    // Check the first word to figure out what type of command this is
    if (words[0] == "INSERT") {
        command.type = CommandType::INSERT;
        command.tableName = words[1];
        
        // stoi() converts a string (like "1") into an actual integer (1)
        command.id = stoi(words[2]); 
        
        // Grab all the remaining words as columns for our Table
        for (int i = 3; i < words.size(); i++) {
            command.columns.push_back(words[i]);
        }
    } 
    else if (words[0] == "SELECT") {
        command.type = CommandType::SELECT;
        command.tableName = words[1];
        command.id = stoi(words[2]); 
    }
    else if (words[0] == "DELETE") {
        command.type = CommandType::DELETE;
        command.tableName = words[1];
        command.id = stoi(words[2]);
    }
    
    return command;
}
