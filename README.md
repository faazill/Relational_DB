# Custom Relational Database Engine 

A lightweight, crash-safe relational database engine built entirely from scratch in C++17. This project was developed to explore the low-level systems architecture of modern databases like PostgreSQL and SQLite.

## Features

* **Write-Ahead Logging (WAL)**: Guarantees 100% data durability and zero-data-loss crash recovery using OS-level file flushing.
* **B+ Tree Indexing**: Custom in-memory B+ Tree implementation for rapid $O(\log n)$ point queries, keeping data strictly sorted.
* **Binary Storage Engine**: Custom `DiskManager` that serializes rows into a compact `.bin` format using raw byte pointers, featuring $O(1)$ tombstone deletions.
* **Lexer & SQL Parser**: Tokenizes and parses English-like SQL commands (e.g., `INSERT`, `SELECT`, `DELETE`) into an Abstract Syntax Tree (AST) for query execution.
* **Interactive REPL**: A command-line interface for live query execution and database interaction.

## Architecture 

```text
[ User REPL ] --> [ SQL Parser (Lexer + AST) ] --> [ Query Executor ]
                                                          |
                                                          v
                                                 [ Relational Table ]
                                                          |
                                                          v
                                              [ Key-Value Storage Layer ]
                                               /          |            \
                                              /           |             \
                               [ Write-Ahead Log ]   [ B+ Tree Index ]   [ Binary Disk Manager ]
                                 (Durability)          (Speed)              (Persistence)
```

## Getting Started

### Prerequisites
* A C++17 compatible compiler (GCC, Clang, MSVC)
* CMake (v3.14+)

### Build Instructions
This project uses CMake to handle the build process and automatically fetches GoogleTest for the testing suite.

```bash
# Clone the repository
git clone https://github.com/your-username/database-engine.git
cd database-engine

# Create build directory and compile
mkdir build
cd build
cmake ..
make
```

### Running the Tests
```bash
# Inside the build directory
./wal_test
./disk_test
./btree_test
./table_test
```

## Usage

Start the interactive REPL (Read-Eval-Print Loop) to interact with the database:

```bash
./database_engine
```

**Example Session:**
```text
======================================
 Welcome to My Custom Database Engine 
======================================
Type commands like:
  INSERT users 1 Alice 25
  SELECT users 1
Type 'exit' to quit.

db> INSERT users 1 Alice 25
Success: Inserted row 1!

db> SELECT users 1
Found Row 1:
- Alice
- 25

db> exit
Goodbye!
```

## Future Roadmap
- [ ] Implement a Buffer Pool Manager with an LRU Eviction Policy to handle datasets larger than RAM.
- [ ] Add support for complex `WHERE` clauses in the AST.
- [ ] Upgrade the custom serialization protocol to support dynamic column schemas.
# Relational_DB
