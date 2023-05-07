#include <iostream> // std::cout
#include <thread> //std::thread::id || std::this_thread::get_id()
#include <chrono>

void Work()
{
    for(int i = 0; i < 20; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout<<i<<std::endl;
    }
}

void GetThreadId()
{
    std::cout<<"Hello World from Thread ID: "<< std::this_thread::get_id()<<std::endl;
}

int main()
{

    std::thread *th = new std::thread(Work); // creates a new thread, and starts it's execution 

    std::thread th2([](){  // using lambda expression [capture](args){code}
        Work();
        //std::cout<<"Hello World from Thread ID: " << std::this_thread::get_id()<<std::endl;
    });

    //std::cout<<"Hello World from Main Thread, Thread ID: " << std::this_thread::get_id() <<std::endl;

    if(th->joinable()) th->join();// wait for the thread th to complete it's execution 
    th2.join();
    return 0;
}