#include "executor.h"
#include <iostream>

using namespace std;

Executor::Executor(Table* dbTable) {
    table = dbTable;
}

void Executor::execute(ParsedCommand command) {
    if (command.type == CommandType::INSERT) {
        table->insertRow(command.id, command.columns);
        cout << "Success: Inserted row " << command.id << "!" << endl;
    } 
    else if (command.type == CommandType::SELECT) {
        optional<Row> result = table->getRow(command.id);
        
        if (result.has_value()) {
            cout << "Found Row " << command.id << ":" << endl;
            // Print out all the columns we found
            for (int i = 0; i < result.value().columns.size(); i++) {
                cout << "- " << result.value().columns[i] << endl;
            }
        } else {
            cout << "Error: Row " << command.id << " not found." << endl;
        }
    }
    else if (command.type == CommandType::DELETE) {
        // Our simple MVP Table class didn't implement delete to keep things easy, 
        // but the Parser recognizes the command!
        cout << "Delete command recognized (Not fully implemented in Table yet)." << endl;
    }
    else {
        cout << "Error: Unknown command." << endl;
    }
}
