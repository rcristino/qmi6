#include <gtest/gtest.h>
#include <future>

#include "Queue.h"

/**
 * Test Case: PushAndPop
 * 
 * Test basic queue operations such as test inserted elements counters and FIFO behaviour
 */
TEST(QueueTest, PushAndPop) {
    Queue<int> q(3);
    q.Push(111);
    auto value = q.GetValue();
    EXPECT_EQ(value, 111);
    EXPECT_EQ(q.Count(), 0);
    EXPECT_EQ(q.Size(), 3);

    q.Push(222);
    q.Push(333);
    value = q.GetValue();
    EXPECT_EQ(value, 222);
    EXPECT_EQ(q.Count(), 1);
    EXPECT_EQ(q.Size(), 3);

    value = q.GetValue();
    EXPECT_EQ(value, 333);
    EXPECT_EQ(q.Count(), 0);
    EXPECT_EQ(q.Size(), 3);
}

/**
 * Test Case: PushAndPopWhenFull
 * 
 * Test basic queue operations but on the limit when the queue is full.
 * In this case The oldest value is dropped from the queue and new one added
 */
TEST(QueueTest, PushAndPopWhenFull) {
    Queue<int> q(3);
    q.Push(111);
    q.Push(222);
    q.Push(333);
    q.Push(444);
    EXPECT_EQ(q.Count(), 3);
    EXPECT_EQ(q.Size(), 3);

    auto value = q.GetValue();
    EXPECT_EQ(value, 222);

    value = q.GetValue();
    EXPECT_EQ(value, 333);

    value = q.GetValue();
    EXPECT_EQ(value, 444);
}

/**
 * Test Case: QueueEmptyAndPop
 * 
 * In case the queue is empty and a pop method is called then it will throw an exception
 */
TEST(QueueTest, QueueEmptyAndPop) {
    Queue<int> q(3);
    EXPECT_THROW(q.GetValue(), std::runtime_error);
}

/**
 * Test Case: QueueEmptyAndPopTimeout
 * 
 * In case the queue is empty and no value is added into the queue,
 *  then timeout will be reached and an exception will be raised
 */
TEST(QueueTest, QueueEmptyAndPopTimeout) {
    Queue<int> q(3);

    // get current time
    auto startTime = std::chrono::steady_clock::now();

    // catch exception because timeout has been reached
    EXPECT_THROW(q.PopWithTimeout(100), std::runtime_error);

    // check if the timeout was reached with a tolerance of 10 milliseconds
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    EXPECT_LT(deltaTime - std::chrono::milliseconds(100), std::chrono::milliseconds(10));
}

/**
 * Test Case: PopBeforeTimeout
 * 
 * In case the queue is empty but a value is added before reaching timeout
 */
TEST(QueueTest, PopBeforeTimeout) {
    Queue<int> q(3);

    // launch PopWithTimeout async task
    auto future = std::async(std::launch::async, &Queue<int>::PopWithTimeout, &q, 500 /* milliseconds*/);

    // get current time
    auto startTime = std::chrono::steady_clock::now();

    // add a delay
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // after delay add a new value
    q.Push(111);

    // get the difference in time between starting the task and adding new value
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);

    // check if a value was retrieved before timeout
    auto status = future.wait_for(std::chrono::milliseconds(5000));
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_LT(deltaTime, std::chrono::milliseconds(500));
    int value = future.get();
    EXPECT_EQ(value, 111);
}

/**
 * Test Case: PopWaitForValue
 * 
 * In case the queue is empty but it will infinitely blocked and waiting for a new value in the queue 
 */
TEST(QueueTest, PopWaitForValue) {
    Queue<int> q(3);

    // launch Pop async task
    auto future = std::async(std::launch::async, &Queue<int>::Pop, &q);

    // get current time
    auto startTime = std::chrono::steady_clock::now();
    
    // delay and add new value into the queue
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    q.Push(111);

    // check if task is unblocked, ended and value retrieved
    auto status = future.wait_for(std::chrono::milliseconds(5000));
    EXPECT_EQ(status, std::future_status::ready);
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    EXPECT_LT(deltaTime, std::chrono::milliseconds(5000));
    EXPECT_LT(deltaTime - std::chrono::milliseconds(100), std::chrono::milliseconds(10));
    int value = future.get();
    EXPECT_EQ(value, 111);
}

// Main function for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}