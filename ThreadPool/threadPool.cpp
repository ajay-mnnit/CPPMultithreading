#include<iostream>
#include<queue>
#include<thread>
#include<future>
#include<chrono>

template<typename T>
class BlockingQueues{
    public:
    BlockingQueues(int size): _size(size){} 

    void push(T item)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _condition.wait(lock, [&]{return _q.size() < _size;});
        _q.push(item);
        std::cout<<"Pushed, Queue size: "<< _q.size()<<std::endl;
        lock.unlock();
        _condition.notify_all();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _condition.wait(lock, [&]{ return !_q.empty();});
        T item = _q.front();
        _q.pop();
        std::cout<<"Popped, Queue size: "<< _q.size()<<std::endl;
        lock.unlock();
        _condition.notify_all();
        return item;
    }

    size_t getSize()
    {
        std::unique_lock<std::mutex> lock (_mtx);
        size_t size = _q.size();
        lock.unlock();
        return size;
    }

    private:
    int _size;
    std::queue<T> _q;
    std::condition_variable _condition;
    std::mutex _mtx;
};

int PerformOperation(std::mutex& m_lock, int num)
{
    // int sum = 0;
    // for(int i = 0; i < range; i++)
    // {
    //     std::this_thread::sleep_for(std::chrono::microseconds(100));
    //     sum += i;
    //     std::cout<<"Thread id: " << std::this_thread::get_id() <<" ,sum: "<< sum <<std::endl;
    // }
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    
    {
        std::lock_guard<std::mutex> lock(m_lock); // lock the block to execute it atomically 
        std::cout<<"PerformOperation, Thread id: " << std::this_thread::get_id()<<std::endl;
    }

    return num;
}


int main()
{
    std::mutex m_lock;
    std::condition_variable cd_var;
    size_t MAXSIZE = 2;
    BlockingQueues<std::shared_future<int>> ThreadPool(MAXSIZE); // no. of threads allow to run at a time

    std::thread masterThread([&]{
        for(int i = 0; i < 200; i++)  // run 40 threads using a threadpool of MAXSIZE
        {  
            std::shared_future<int> Future = std::async(std::launch::async, PerformOperation, std::ref(m_lock), i);
            ThreadPool.push(Future); 
        }
    });


    for(int i = 0; i < 200; i++)
    { 
        std::shared_future<int> Future = ThreadPool.pop(); 
        std::cout<<"Future Value: "<<Future.get();
    }

    masterThread.join();
    return 0;
}
