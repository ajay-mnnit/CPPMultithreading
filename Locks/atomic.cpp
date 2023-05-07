#include <iostream>
#include <thread>
#include <atomic>

int main ()
{
    //int count = 0; // shared elemnet, RACE AROUND CONDITON occurs here
    std:: atomic<int> count = 0; // atomic makes the variable 
    /*
    std::atomic is a type template that provides a way to perform atomic operations on shared data
    without the need for locks or other synchronization mechanisms. 
    Atomic operations are those that are guaranteed to execute without interruption from other threads,
    thus ensuring that the value being manipulated is always consistent.*/
    
    int OPERATIONS = 1e7;

    std::thread t1 ([&]{
        for(int i = 0; i < OPERATIONS; i++)
        {
            count++;
        }
    });

     std::thread t2 ([&]{
        for(int i = 0; i < OPERATIONS; i++)
        {
            count++;
        }
    });

    t1.join();
    t2.join();
    std::cout<<count<<std::endl;
}