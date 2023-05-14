/*
std::async is a function in C++ that allows you to run a function asynchronously in a separate thread. 
It returns a std::future object that represents the result of the function call, which can be retrieved at a later time.
*/

/*
The function can be launched in three different modes:

std::launch::async: This mode requires that the function is run asynchronously, i.e., on a separate thread. If the function is already running on a separate thread, then a new thread may not be created.

std::launch::deferred: This mode allows the function to be executed synchronously, i.e., on the same thread that calls std::async. The function is executed only when the std::future object is accessed or when the std::future destructor is called.

std::launch::async | std::launch::deferred: This mode allows the implementation to choose either of the above modes.
       OR
std::launch::any
*/

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int Func(int range)
{
    int sum = 0;

    for(int i = 0; i < range; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout<<"Thread: " <<std::this_thread::get_id()<< ", value: "<< i<<std::endl;
    } 
    return sum;
}


int main()
{
    std::cout<<"Main thread Id: "<<std::this_thread::get_id()<<std::endl;

    //lunching async in deferred mode, future object is not must in this case, it will run asynchronously in different thread
    // thread show async behaviour if future.get() method is being called 
    //std::future<int> f1 = std::async(std::launch::async, Func, 100);
    //std::future<int> f2 = std::async(std::launch::async, Func, 100);

    // luanching async in deferred mode, future object get() is must, runs in the same thread(main thread)  
    // std::future<int> f1 = std::async(std::launch::deferred, Func, 100);
    // std::future<int> f2 = std::async(std::launch::deferred, Func, 100); 
    // f1.get(); 
    // f2.get(); 


    std::future<int> f1 = std::async(std::launch::async | std::launch::deferred, Func, 100);
    std::future<int> f2 = std::async(std::launch::async | std::launch::deferred, Func, 100);
    f1.get(); 
    f2.get();
    return 0;
}
