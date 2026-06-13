#include <iostream>
#include <string>
#include "storage/kv_store.h"
#include "storage/table.h"
#include "parser/sql_parser.h"
#include "executor/executor.h"

using namespace std;

int main() {
    cout << "======================================" << endl;
    cout << " Welcome to My Custom Database Engine " << endl;
    cout << "======================================" << endl;
    cout << "Type commands like:" << endl;
    cout << "  INSERT users 1 Alice 25" << endl;
    cout << "  SELECT users 1" << endl;
    cout << "Type 'exit' to quit." << endl;

    // 1. Boot up the Storage Engine
    KeyValueStore store;
    Table usersTable("users", &store);
    
    // 2. Boot up the Executor
    Executor executor(&usersTable);

    // 3. The REPL (Read-Eval-Print Loop)
    string input;
    while (true) {
        cout << "\ndb> ";
        getline(cin, input); // Wait for the user to type something

        if (input == "exit" || input == "quit") {
            break; // Stop the while loop
        }
        if (input.empty()) {
            continue; // They just pressed Enter, ask again
        }

        // 4. Parse the input
        SQLParser parser(input);
        ParsedCommand command = parser.parse();

        // 5. Execute it!
        executor.execute(command);
    }

    cout << "Goodbye!" << endl;
    return 0;
}
