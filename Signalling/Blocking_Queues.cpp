#include <iostream>
#include <thread> //thread class   
#include <atomic> //atomic type shared resource
#include <chrono> // std::chrono::seconds
#include <condition_variable> //std::condition_variable class
#include <mutex> //std::mutex class
#include <queue>


template<typename T>
class BlockingQueues{
    public:
    BlockingQueues(int size): _size(size){} 

    void push(T item)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        std::cout<<"Pushing element: "<<item<<std::endl;
        _condition.wait(lock, [&]{return _q.size() < _size;});
        _q.push(item);
        lock.unlock();
        _condition.notify_all();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _condition.wait(lock, [&]{ return !_q.empty();});
        T item = _q.front();
        _q.pop();
        lock.unlock();
        _condition.notify_all(); 
        return item;
    }

    T getSize()
    {
        return _q.size();
    }

    private:
    int _size;
    std::queue<T> _q;
    std::condition_variable _condition;
    std::mutex _mtx;
};

int main()
{
    BlockingQueues<int> Q(3);
 
    std::thread t1([&]
    {
        for(int i = 0; i < 100; i++)
        {
            Q.push(i);
            std::cout<<"size: "<< Q.getSize()<<std::endl;
        }
    }); 

    std::thread t2([&]
    {
        for(int i = 0; i < 100; i++)
        { 
            int item = Q.pop();
            std::cout<<"Poping out element: " << item << std::endl;
        }
    }); 

    t1.join();
    t2.join();

    return 0;
}
