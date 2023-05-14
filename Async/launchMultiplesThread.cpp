#include<iostream>
#include<thread>
#include <vector>
#include<future>
#include <mutex>

std::mutex m_lock;

int func(int curr)
{
    std::unique_lock<std::mutex>lock (m_lock);
    int sum = 0;

    for(int i = 0; i <= curr; i++)
    {
        sum += i;
    }
    std::cout<<"Thread id: "<< std::this_thread::get_id()<< ", sum: "<< sum <<std::endl;
    lock.unlock();

    return sum;
}

int main()
{
    std::vector<std::shared_future<int>> Futures;

    for(int i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        std::shared_future<int> f = std::async(std::launch::async, func, i); 
        Futures.push_back(f);
    }

    for(auto &&p: Futures)
    {
        //p.get(); // dont need to get values in case of async launch, getting future values is enough to run it asynchronously 
    }
    return 0;
}   
