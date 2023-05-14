#include <iostream>
#include <thread> //thread class   
#include <atomic> //atomic type shared resource
#include <chrono> // std::chrono::seconds


int main()
{
    std::atomic<bool> flag = false; 

    std::thread t1([&]{
        //std::this_thread is a namespace groups a set of functions that access the current thread.
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        flag = true;
    });

    while(!flag) // main thread waits for the t1, till it changes the flag true
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout<<"waiting for flag"<<std::endl;
    }

    t1.join();
    std::cout<<"flag: "<<flag<<std::endl;
    return 0;
}
