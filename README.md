# Q MI6

## Queue Project

This project implements a thread-safe queue using a linked list in C++. 
The Queue class provides basic queue operations, such as push and pop elements
This Queue is initialized with a finite size accepting only primitive types such as int, float, double, long, short.
The implementation ensures thread safety using mutexes.
In case queue is full, pushing a new element shall drop the oldest element in-queue before storing a new one
Code is documented using Doxygen style

## Features

- **Thread-Safe**: Ensures safe concurrent access to the queue.
- **FIFO**: Implements a First-In-First-Out queue structure.
- **Dynamic Memory Allocation**: Uses dynamic memory for storing queue elements.
- **Unit Tests**: Unit Tests to confirm the right queue behavior in different scenarios and operations
- **Documentation**: The source code is commented using Doxygen style

## Getting Started

### Prerequisites

- C++ Compiler (e.g., g++, clang++)
- CMake (for build automation)
- Google Test (for unit testing)
- Doxygen (optional, to produce html documentation)

### Project step by step

1. **Clone the Repository**

   ```bash
   git clone https://github.com/rcristino/qmi6.git
   cd qmi6

2. **Build Project**

   ```bash
   mkdir build
    cd build
    cmake ..
    make

3. **Run Project**

   ```bash
   ./build/qmi6

4. **Run Test Cases**

   ```bash
   ./build/qmi6Test

4. **Generate Documentation**

   ```bash
   ./runDoxygen.sh