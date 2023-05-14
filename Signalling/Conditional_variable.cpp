/*
In C++ multithreading, a std::condition_variable is a synchronization primitive that allows one or 
more threads to wait for a notification from another thread. It is typically used in combination with a 
std::unique_lock or std::lock_guard to protect shared data from race conditions.

A std::condition_variable is a part of the <condition_variable> header 
works by waiting on a shared variable using a std::unique_lock and a lambda expression that returns a boolean value. 
The waiting thread is blocked until the shared variable is changed and the lambda expression returns true.
*/

////////////////////////////////////////////////////////////// from CPPReference.com
/*
The condition_variable class is a synchronization primitive used with a std::mutex to block one or more threads until 
another thread both modifies a shared variable (the condition) and notifies the condition_variable.

The thread that intends to modify the shared variable must:

Acquire a std::mutex (typically via std::lock_guard)
Modify the shared variable while the lock is owned
Call notify_one or notify_all on the std::condition_variable (can be done after releasing the lock)
Even if the shared variable is atomic, it must be modified while owning the mutex to correctly publish the modification to the waiting thread.

Any thread that intends to wait on a std::condition_variable must:

Acquire a std::unique_lock<std::mutex> on the mutex used to protect the shared variable
Do one of the following:
Check the condition, in case it was already updated and notified
Call wait, wait_for, or wait_until on the std::condition_variable (atomically releases the mutex and suspends 
thread execution until the condition variable is notified, a timeout expires, or a spurious wakeup occurs, then atomically acquires the mutex before returning)
Check the condition and resume waiting if not satisfied 
*/


#include <iostream>
#include <thread> //thread class   
#include <atomic> //atomic type shared resource
#include <chrono> // std::chrono::seconds
#include <condition_variable> //std::condition_variable class
#include <mutex> //std::mutex class

int main()
{
    bool flag = false;
    std::mutex mtx;
    std::condition_variable condition;

    std::thread t1 ([&]{
        std::this_thread::sleep_for(std::chrono::seconds(2)); //sleep for 2 seconds, main thread acquire the lock
        std::unique_lock<std::mutex> lock(mtx); // acquiring the lock
        flag = true; // updating shared varialbe
        lock.unlock(); // unlocking the lock
        condition.notify_one(); //notifying other thread for aquire the lock
    });

    //std::unique_lock<std::mutex> lock(mtx); // main thread acquires the lock 
    // while(!flag)
    // {
    //     condition.wait(lock); // if condtion is not true, releasing the lock for other threads
    // }

    //OR 

    std::unique_lock<std::mutex> lock(mtx); // main thread acquires the lock 
    condition.wait(lock, [&]{
        return flag;
    });// wait till the lambda expression returns true
 
    std::cout<<"flag: "<<flag<< std::endl;
    t1.join();

    return 0;
}
