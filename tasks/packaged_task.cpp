/* 
In C++ multithreading, a std::packaged_task is a template class that wraps a callable object, such as a function or a lambda expression, 
and provides a way to launch it asynchronously and retrieve its result later through a std::future object.

The std::packaged_task class is similar to std::function, but with the additional ability to execute the wrapped function asynchronously in a separate thread. 
It is useful when you want to run a function in a separate thread and obtain its return value or exception status.

template< class R, class ...ArgTypes >
class packaged_task<R(ArgTypes...)>;
*/

#include <iostream>
#include <thread> //thread class
#include <iomanip> // std::setprecision() method
#include <future> // promise and future classes
#include <cmath> // pow function 
#include <exception> //std::exception


double CalculatePI(int termsUpTo)
{
    if(termsUpTo <= 0)
        throw  std::runtime_error("TERM should be more than 0");

    double ans = 0.0;
    for(int i = 0; i < termsUpTo; i++)
    {
        int sign = std::pow(-1, i);
        double curr = 1.0/(2*i + 1);
        ans += sign*curr;
    } 
    return ans*4;
}


int main()
{
    std::future<double> Future;

    std::packaged_task<double(int)> task(CalculatePI); //define a packaged task
    Future = task.get_future();// get the future object  

    std::thread t1(std::move(task),1e8); // move the task object, since it's no longer needed in this thread

    try
    {
       double ans = Future.get(); // get the result from the future object 
       std::cout<<std::setprecision(15)<<ans <<std::endl; 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if(t1.joinable())
        t1.join();

    return 0; 
}
