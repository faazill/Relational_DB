cmake_minimum_required(VERSION 3.14)
project(DatabaseEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src)

# Combine our WAL code and Test code into a single executable
add_executable(wal_test tests/wal_test.cpp src/wal/wal_manager.cpp)

# Add our new Disk Manager test executable
add_executable(disk_test tests/disk_manager_test.cpp src/storage/disk_manager.cpp)

# Add our new B+ Tree test executable
add_executable(btree_test tests/btree_test.cpp src/btree/bplus_tree.cpp)

# Add our Full Storage Engine test
add_executable(table_test tests/table_test.cpp src/storage/table.cpp src/storage/kv_store.cpp src/wal/wal_manager.cpp src/storage/disk_manager.cpp src/btree/bplus_tree.cpp)

# Add the Main Application
add_executable(database_engine src/main.cpp src/executor/executor.cpp src/parser/sql_parser.cpp src/storage/table.cpp src/storage/kv_store.cpp src/wal/wal_manager.cpp src/storage/disk_manager.cpp src/btree/bplus_tree.cpp)
