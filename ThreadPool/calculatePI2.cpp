#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <cmath>
#include <iomanip> // std::setprecision() method

std::mutex _m_lock;
 
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
    size_t MAXTHREAD = std::thread::hardware_concurrency(); // no of threads supported by system at a time
    std::vector<std::shared_future<double>> Futures; //store the future object of the asyc method to acess the results of each thread

    double ans = 0.0;
    
    std::cout<<"Started, Total threads " <<MAXTHREAD<<std::endl; 

    auto start = std::chrono::steady_clock:: now();
    for(int i = 0; i < MAXTHREAD; i++)  // run threads =  MAXSIZE
    {  
        std::shared_future<double> Future = std::async(std::launch::async, CalculatePI, 1E7, i, MAXTHREAD);
        Futures.push_back(Future);
    } 

    for(auto F: Futures)
    {  
        ans += F.get(); 
    }
    auto end = std::chrono::steady_clock:: now();
    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout<<"Ended"<<std::endl<<"Total Time Takes: "<<totalTime<<std::endl;

    std::cout<<std::setprecision(15)<<ans<<std::endl;
    return 0;
}
