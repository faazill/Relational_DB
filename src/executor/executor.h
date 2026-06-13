#pragma once
#include "../parser/sql_parser.h"
#include "../storage/table.h"

class Executor {
public:
    // The Executor needs to know which Table it is talking to
    Executor(Table* dbTable);
    
    // Takes the parsed command and actually executes it on our database
    void execute(ParsedCommand command);

private:
    Table* table;
};
