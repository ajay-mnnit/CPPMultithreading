/*
==> Locks are used to avoid release mutexes when it goes out of scope or execption occurs at shared resources  

    LOCKS==> std::lock_guard and std::unique_lock are two types of lock guards in C++ that provide automatic locking and unlocking of mutexes.

    STD::LOCK_GUARD===>
    std::lock_guard is a simple lock guard that provides a scoped lock for a mutex. 
    When a std::lock_guard object is created with a mutex object as its argument, 
    it acquires the lock on the mutex, and when the std::lock_guard object goes out of scope, 
    the lock is automatically released. This ensures that the lock is held for the duration of the std::lock_guard object's lifetime, 
    and no other thread can acquire the lock until the std::lock_guard is destructed.

    STD::UNIQUE_LOCK==>
    std::unique_lock is a more versatile lock guard that provides the same automatic locking and unlocking of mutexes, 
    but also allows more control over the lock. Unlike std::lock_guard, std::unique_lock can be created with a std::defer_lock argument, 
    which means that the lock is not acquired immediately, but can be acquired later with the lock() method. 
    std::unique_lock also provides a try_lock() method that attempts to acquire the lock without blocking, and a unlock() method that releases the lock.
    
    // Create a unique_lock with defer_lock
    std::unique_lock<std::mutex> lock(my_mutex, std::defer_lock);

    // Do some work here that doesn't require the lock
    // ...

    // Acquire the lock when necessary
    lock.lock();

    // Update the shared variable
    my_variable = 42;

    // Release the lock
    lock.unlock();

    // Do some other work here that doesn't require the lock
    // ...

    // Acquire the lock again
    lock.lock();

    // Read the shared variable
    std::cout << "my_variable = " << my_variable << std::endl;

    // Release the lock
    lock.unlock();

*/


#include <iostream>
#include <thread>
#include <mutex>


void Work(std::mutex &mtx, int &count)
{
    for(int i = 0; i < 1E7; i++)
    {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        //std::lock_guard<std::mutex> lock(mtx);  // don't have access on lock, it unlocks when it goes out of scope
        lock.lock();
        count++;
        //lock.unlock();
    }   
}

int main()
{
    std::mutex mtx;
    int count = 0;
     
    std::thread t1(Work, std::ref(mtx), std::ref(count));
    std::thread t2(Work, std::ref(mtx), std::ref(count));
 
    t1.join();
    t2.join();

    std::cout<< count<< std::endl;

    return 0;
}
