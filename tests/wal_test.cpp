#include <iostream>
#include "../src/wal/wal_manager.h" // Include our new code

using namespace std;

int main() {
    cout << "--- Testing the Write-Ahead Log ---" << endl;

    // 1. Create a log file called "test_log.txt"
    WriteAheadLog wal("test_log.txt");

    // 2. Add some fake crash-recovery data
    wal.addLog("INSERT", "users", 1, "Alice");
    wal.addLog("INSERT", "users", 2, "Bob");
    wal.addLog("DELETE", "users", 1, "Alice");

    cout << "Added 3 logs to test_log.txt!" << endl;

    // 3. Now pretend the database crashed and restarted.
    // Let's read all the logs back from the file.
    vector<LogRecord> recoveredLogs = wal.readAllLogs();

    cout << "\nRecovered Logs:" << endl;
    for (int i = 0; i < recoveredLogs.size(); i++) {
        LogRecord rec = recoveredLogs[i];
        cout << "- " << rec.operation << " on table '" << rec.tableName 
             << "' for ID " << rec.rowId << " (Data: " << rec.rowData << ")" << endl;
    }

    // 4. Clean up the file so it's fresh for the next time we run the test
    wal.clearLog();
    cout << "\nTest complete. Log file cleared." << endl;

    return 0;
}
