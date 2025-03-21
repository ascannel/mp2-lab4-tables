# Data Structures Library in C++

![C++](https://img.shields.io/badge/C%2B%2B-11%2B-blue)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-brightgreen)

A comprehensive library implementing various data structures including hash tables, binary trees, AVL trees, red-black trees, and sorted/simple tables. Designed for educational and practical use cases.

## Features

- **Tables**:
  - `SimpleTable`: Unsorted key-value storage with basic operations.
  - `SortTable`: Sorted key-value storage using binary search for efficient lookups.
  - `HashTable`: Configurable hash table with collision handling and dynamic resizing.
- **Trees**:
  - `BinaryTree`: Standard binary search tree.
  - `AVLTree`: Self-balancing AVL tree with height management.
  - `RBTree`: Red-black tree with color-based balancing.
- **Iterators**: Unified iterators for traversal and manipulation.
- **Tests**: Extensive Google Tests for validation and performance benchmarking.

## Dependencies 
- CMake (>= 3.10)
- C++17-compatible compiler
- Google Test (included as submodule in the project structure)

## Documentation
Code Structure:
- `iterator.hpp`: Base iterators for all containers.
- `table.hpp`: Implementations of tables and trees.
- `test_tables.cpp`: Validation and performance tests.

API Details: All classes expose standard methods (insert, remove, find, iterators).

## Testing
Run all tests after building:

```bash
./bin/test_tables
```
