#include "Queue.h"

#include <iostream>
#include <chrono>
#include <future>

// Explicit instantiation for long
template class Queue<long>;

// Explicit instantiation for int
template class Queue<int>;

// Explicit instantiation for double
template class Queue<double>;

// Explicit instantiation for float
template class Queue<float>;

// Explicit instantiation for short
template class Queue<short>;

template <typename T>
Queue<T>::Queue(const unsigned int size)
: queueSize(size)
, backPtr(nullptr)
, frontPtr(nullptr) {}

template <typename T>
Queue<T>::~Queue<T>() {
    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutexQueue);

    // Iterate and delete all elements from the queue
    while (backPtr != nullptr) {
        Element* tempPtr = backPtr;
        backPtr = backPtr->nextPtr;
        delete tempPtr;
    }
}

template <typename T>
void Queue<T>::Push(const T value) {
    // Create new element to be added to the queue
    Element* newElementPtr = new Element();
    newElementPtr->value = value;
    newElementPtr->nextPtr = nullptr;

    // Check if queue is not full
    if(Count() < Size()) {
        // Lock the mutex
        std::lock_guard<std::mutex> lock(mutexQueue);

        // Check if queue is empty
        if (frontPtr == nullptr) {
            // First element in the queue
            backPtr = frontPtr = newElementPtr;
        } else {
            // Add new element at the front of the queue
            frontPtr->nextPtr = newElementPtr;
            frontPtr = newElementPtr;
        }

        // Update the queue elments counter
        ++queueCount;
    }
    else
    {
        // Lock the mutex
        std::lock_guard<std::mutex> lock(mutexQueue);

        // Get the first element from the queue
        Element* tempElementPtr = backPtr;
        backPtr = backPtr->nextPtr;
        
        // Add new element into the front of the queue
        frontPtr->nextPtr = newElementPtr;
        frontPtr = newElementPtr;

        // Free the first element to give room to new element (queue is full)
        delete tempElementPtr;
    }
}

template <typename T>
T Queue<T>::GetValue() {
    // Value to be returned
    T value;

    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutexQueue);

    // Check if queue is empty
    if (backPtr == nullptr) {
        throw std::runtime_error("Queue is empty.");
    }

    // Swap the front element from the queue
    Element* tempPtr = backPtr;
    backPtr = backPtr->nextPtr;

    // Check if it is the last element of the queue
    if (backPtr == nullptr) {
        frontPtr = nullptr;
    }

    // Set the value to be returned and delete element
    value = tempPtr->value;
    delete tempPtr;

    // Update the queue counter and notify
    --queueCount;

    return value;
}

template <typename T>
T Queue<T>::Pop() {
    // Value to be returned
    T value;

    // Lambda to start the asynchronous task and get value from queue 
    auto startAsyncTask = [&]() {
        std::future<T> future = std::async(std::launch::async, &Queue<T>::GetValue, this);
        return future;
    };

    // Launch task
    std::future<T> future = startAsyncTask();
    
    // Wait for the queue value or timeout
    bool waitingForValue = true;
    while(waitingForValue) {
        future.wait();
        try {
            value = future.get();
            waitingForValue = false;
        } catch (const std::exception& e) {
            // std::cerr << "Exception: " << e.what() << std::endl;
            future = startAsyncTask();
        }
    }

    return value;
}

template <typename T>
T Queue<T>::PopWithTimeout(const unsigned int milliseconds) {
    // Value to be returned
    T value;

    // Record the start time
    auto startTime = std::chrono::steady_clock::now();

    // Set timeout duration
    std::chrono::milliseconds timeout(milliseconds);

    // Lambda to start the asynchronous task and get value from queue 
    auto startAsyncTask = [&]() {
        std::future<T> future = std::async(std::launch::async, &Queue<T>::GetValue, this);
        return future;
    };

    // Launch task
    std::future<T> future = startAsyncTask();
    
    // Wait for the queue value or timeout
    bool waitingForValue = true;
    while(waitingForValue) {
        auto deltaTime = std::chrono::steady_clock::now() - startTime;
        if (future.wait_for(timeout - deltaTime) == std::future_status::ready) {
            try {
                value = future.get();
                waitingForValue = false;
            } catch (const std::exception& e) {
                // std::cerr << "Exception: " << e.what() << std::endl;
                future = startAsyncTask();
            }   
        } else {
            // Timeout expired before a value has been pop from the queue
            throw std::runtime_error("No value within the timeout period.");
        }
    }

    return value;
}

template <typename T>
unsigned int Queue<T>::Count() {
    // Lock the mutex
    std::lock_guard<std::mutex> lock(mutexQueue);
    return queueCount;
}

template <typename T>
unsigned int Queue<T>::Size() const {
    return queueSize;
}