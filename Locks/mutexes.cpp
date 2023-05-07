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