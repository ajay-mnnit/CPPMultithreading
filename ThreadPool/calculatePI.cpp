#include<iostream>
#include<queue>
#include<thread>
#include<future>
#include<chrono>
#include<cmath>
#include <iomanip> // std::setprecision() method

std::mutex _m_lock;

template<typename T>
class BlockingQueues{
    public:
    BlockingQueues(int size): _size(size){} 

    void push(T item)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _condition.wait(lock, [&]{return _q.size() < _size;});
        _q.push(item);
        //std::cout<<"Pushed, Queue size: "<< _q.size()<<std::endl;
        lock.unlock();
        _condition.notify_all();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _condition.wait(lock, [&]{ return !_q.empty();});
        T item = _q.front();
        _q.pop();
       //std::cout<<"Popped, Queue size: "<< _q.size()<<std::endl;
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

///////////////////////////////////////////////////////////////////////////

double CalculatePI(int TERMS, int start, int next)
{
    if(TERMS <= 0)
        throw  std::runtime_error("TERM should be more than 0");

    double ans = 0.0;
    for(int i = start; i < TERMS; i += next)
    {
        int sign = std::pow(-1, i);
        double curr = 1.0/(2*i + 1);
        ans += sign*curr;
    } 
    return ans*4;
}

//////////////////////////////////////////////////////////////////////////

int main()
{
    std::mutex m_lock;
    std::condition_variable cd_var;
    size_t MAXSIZE = 10; // no of threads in the ThreadPool
    BlockingQueues<std::shared_future<double>> ThreadPool(MAXSIZE); // no. of threads allow to run at a time
    double ans = 0.0;

    std::thread masterThread([&]{
        for(int i = 0; i < 10; i++)  // run threads using a threadpool of MAXSIZE
        {  
            std::shared_future<double> Future = std::async(std::launch::async, CalculatePI, 1E6, i, 10);
            ThreadPool.push(Future);
        }
    });

    for(int i = 1; i <= 10; i++)
    { 
        std::shared_future<double> Future = ThreadPool.pop(); 
        ans += Future.get();
    }

    masterThread.join();
    std::cout<<std::setprecision(15)<<ans<<std::endl;
    return 0;
}
