# CMake minimum required version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Qmi6 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Threads package
find_package(Threads REQUIRED)

# Find Google Test
find_package(GTest REQUIRED)

# Define the source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Queue.cpp
)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Add library (STATIC)
add_library(QueueLib STATIC ${SOURCES})

# Add the executable
add_executable(qmi6 src/main.cpp)

# Link the library with the executable
target_link_libraries(qmi6
    PUBLIC 
        QueueLib
    PRIVATE
        Threads::Threads
)

# Add your queue source files here
add_executable(qmi6Test ${SOURCES} tests/QueueTest.cpp)

target_link_libraries(qmi6Test
    PUBLIC 
        QueueLib
    PRIVATE
        Threads::Threads
        GTest::gtest_main 
        GTest::gtest
)