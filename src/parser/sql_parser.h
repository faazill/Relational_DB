#pragma once
#include <string>
#include <vector>

using namespace std;

// This represents the "Type" of command the user typed
enum class CommandType {
    UNKNOWN,
    INSERT,
    SELECT,
    DELETE
};

// This struct holds the "parsed" and understood command (The AST)
struct ParsedCommand {
    CommandType type;
    string tableName;
    int id;
    vector<string> columns; // Used for saving data like "Alice" and "25"
};

class SQLParser {
public:
    // Takes the raw string the user typed (e.g. "INSERT users 1 Alice 25")
    SQLParser(string query);

    // Breaks the string into words (Tokens)
    vector<string> tokenize();
    
    // Turns the words into a command the computer understands (The AST)
    ParsedCommand parse();

private:
    string rawQuery;
};
