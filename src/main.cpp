#include <iostream>
#include <chrono>
#include <future>

#include "Queue.h"

int main() {
    Queue<int> intQueue(3);

    bool keepRunning = true;

    std::future<int> future;
    try {
        // case PopWithTimeout
        while(keepRunning) {
            // async task to pop value. It throws timeout error when no new value in the queue has been added in that period
            future = std::async(std::launch::async, &Queue<int>::PopWithTimeout, &intQueue, 500 /* milliseconds*/);

            // add a delay to have the PopWithTimeout blocked waiting for new elements, then call push to add one
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // delay
            intQueue.Push(111);

            // check if PopWithTimeout is unblocked, the tasks has ended and value retrieved
            if(future.wait_for(std::chrono::milliseconds(5000)) == std::future_status::ready) {
                int value = future.get();
                std::cout << "PopWithTimeout: Value from queue: " << value << std::endl;
                keepRunning = false;
            }
        }

        // case Pop
        keepRunning = true;
        while(keepRunning) {
            // async task to pop value. This task will be blocked until a new value is added into the queue
            future = std::async(std::launch::async, &Queue<int>::Pop, &intQueue);

            // add a deplay to have the Pop task blocked, waiting for new value in the queue, then push a value
            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // delay
            intQueue.Push(111);

            // check if PopWithTimeout is unblocked, the tasks has ended and value retrieved 
            if(future.wait_for(std::chrono::milliseconds(5000)) == std::future_status::ready) {
                int value = future.get();
                std::cout << "Pop: Value from queue: " << value << std::endl;
                keepRunning = false;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in main(): " << e.what() << std::endl;
    }

    return 0;
}
