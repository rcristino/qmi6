/**
 * @file Queue.h
 * @brief Definition of the Queue class with basic queue operations.
 */

#pragma once

#include <mutex>

/**
 * @class Queue
 * @brief A thread-safe queue implemented using a linked list.
 *
 * The Queue class implements a simple FIFO (First-In-First-Out) queue using a linked list. 
 * It provides thread-safe operations for push and pop elements, as well as checking if the queue is empty.
 */
template<class T> 
class Queue {
public:
    /**
     * @brief Constructs an empty Queue.
     *
     * Initializes an empty queue for specific number of elements.
     * 
      @param size Specific queue size.
     */
    Queue<T>(const unsigned int size);
    
    /**
     * @brief Destroys the Queue and frees all allocated resources.
     *
     * The destructor ensures that all elements in the linked list are properly deleted to avoid memory leaks.
     */
    ~Queue<T>();

    /**
     * @brief Adds a new element to the front of the queue.
     *
     * This method inserts a new element at the end of the queue.
     * 
     * @param value The value to be added to the queue.
     */
    void Push(const T value);

    /**
     * @brief Consumes oldest element from the queue and returns value and gets blocked if queue is empty.
     * 
     * This method removes the element from the queue. 
     */
    T Pop();

    /**
     * @brief Consumes oldest element from the queue and returns its value or throws timeout exception.
     *
     * This method removes the element from the queue.
     * If value is not available in the queue in timeout period then and exception is raised.
     * 
     * @throws std::runtime_error Timeout has been reached because no value was added in the queue in that period.
     * @return The oldest value from the queue.
     */
    T GetValue();
    
    /**
     * @brief Consumes oldest element from the queue and returns its value .
     *
     * This method removes the element from the queue. 
     * 
     * @param milliseconds The time to wait to have e value in the queue
     * @throws std::runtime_error If the queue is empty.
     * @return The oldest value from the queue.
     */
    T PopWithTimeout(const unsigned int milliseconds);

    /**
     * @brief Amount of elements currently stored in queue now..
     * 
     * @return Number of elements in the queue.
     */
    // Amount of elements stored now
    unsigned int Count();

    /**
     * @brief The Queue size.
     * 
     * When the queue is full, it drops the oldest value to give room to the new value.
     * 
     * @return Number of elements which can be stored in the queue.
     */
    unsigned int Size() const;

private:
    /**
     * @struct Element
     * @brief Represents a element in the linked list.
     *
     * This internal struct represents a element in the linked list used by the Queue class. 
     * Each element holds a value and a pointer to the next element in the list.
     */
    struct Element {
        T value;  ///< The data value of the node.
        Element* nextPtr;  ///< Pointer to the next element in the list.
    };

    Element* backPtr; ///< Pointer to the back of the queue (oldest element).

    Element* frontPtr; ///< Pointer to the front of the queue (newest element).

    unsigned int queueSize{0u}; ///< Queue maximum size

    unsigned int queueCount{0u}; ///< Current number of elements in the queue

    std::mutex mutexQueue;  ///< Mutex for synchronizing access to the queue
};