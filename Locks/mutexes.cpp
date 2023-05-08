/*
1. std::mutex: This is the most basic type of mutex in C++. It provides exclusive access to shared data, 
    ensuring that only one thread can access the data at a time.

2. std::recursive_mutex: This type of mutex allows a thread to acquire the same lock multiple times without causing a deadlock. 
   This can be useful when a function calls itself recursively and needs to access the same shared data.

3 . std::timed_mutex: This type of mutex allows a thread to wait for a specified amount of time to acquire the lock, 
    instead of blocking indefinitely. This can be useful in situations where a thread needs to acquire a lock, but cannot wait indefinitely.

4 . std::recursive_timed_mutex: This type of mutex combines the features of std::recursive_mutex and std::timed_mutex,
    allowing a thread to acquire the same lock multiple times and wait for a specified amount of time to acquire the lock.

5. std::shared_mutex: This type of mutex allows multiple threads to read the shared data simultaneously, 
    but only one thread can write to the data at a time. This can improve performance in situations where reads are more frequent than writes.

6. std::shared_timed_mutex: This type of mutex combines the features of std::shared_mutex and std::timed_mutex, 
    allowing multiple threads to read the shared data simultaneously and wait for a specified amount of time to acquire the lock for reading or writing.

*/



#include <iostream>
#include <thread>
#include <mutex>

int main ()
{
    int count = 0;
    int OPERATIONS = 1E7;
    std::mutex m;

    auto func = [&]{
        for(int i = 0; i < OPERATIONS; i++)
        {
            m.lock();
            //m.lock(); do not lock a mutex multiple times, it may cause deadlock
            count ++;   // critical section, multiple thread might access this variable #RACEAROUND may occur
            m.unlock();
        }
    };

    std::thread t1(func);
    std::thread t2(func);
    
    t1.join();
    t2.join();

    std::cout<< count<<std::endl;

    return 0;
}