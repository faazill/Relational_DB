#include <iostream>
#include "../src/storage/table.h"
#include "../src/storage/kv_store.h"

using namespace std;

int main() {
    cout << "--- Testing the Full Storage Engine! ---" << endl;

    // 1. Create our Delivery Truck (This automatically starts the WAL, Disk, and B+ Tree!)
    KeyValueStore store;
    
    // 2. Create a "users" table and connect it to the truck
    Table usersTable("users", &store);

    // 3. Insert some rows using multiple columns
    cout << "Inserting Alice and Bob..." << endl;
    
    vector<string> aliceData = {"Alice", "25", "alice@email.com"};
    usersTable.insertRow(1, aliceData);
    
    vector<string> bobData = {"Bob", "30", "bob@email.com"};
    usersTable.insertRow(2, bobData);

    // 4. Retrieve and unpack the rows
    optional<Row> result = usersTable.getRow(1);
    
    if (result.has_value()) {
        cout << "\nFound User ID: " << result.value().id << endl;
        cout << "Name: " << result.value().columns[0] << endl;
        cout << "Age: " << result.value().columns[1] << endl;
        cout << "Email: " << result.value().columns[2] << endl;
    } else {
        cout << "Failed to find user 1!" << endl;
    }

    return 0;
}
