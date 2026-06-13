#include <iostream>
#include "../src/storage/disk_manager.h"

using namespace std;

int main() {
    cout << "--- Testing the Disk Manager ---" << endl;

    // 1. Create a binary database file called "test_db.bin"
    DiskManager disk("test_db.bin");

    // 2. Insert some rows
    disk.insertRow(100, "Apple");
    disk.insertRow(200, "Banana");
    
    // Let's update Apple to be a Green Apple! 
    // (We just insert ID 100 again, and it will overwrite it in our read logic)
    disk.insertRow(100, "Green Apple");
    
    // Let's delete Banana
    disk.deleteRow(200);

    // 3. Read everything back into RAM
    vector<SavedRow> allRows = disk.readAllRows();

    cout << "Final Database State:" << endl;
    for (int i = 0; i < allRows.size(); i++) {
        cout << "- ID: " << allRows[i].id << " | Data: " << allRows[i].data << endl;
    }

    // You should ONLY see "Green Apple" with ID 100 printed.
    // Banana was deleted, and the original Apple was overwritten!

    return 0;
}
