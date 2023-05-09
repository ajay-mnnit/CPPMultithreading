/*

Promises and futures are mechanisms in C++ that allow asynchronous communication between threads. 
The basic idea is that one thread can "promise" to provide a value or exception at some point in the future, 
and another thread can "wait" for that value or exception using a "future".

A promise is an object that can be used to store a value or exception that will be made available at some point in the future. 
A future is an object that can be used to retrieve the value or exception that was promised by the promise.

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
    //////////////////////////////////////////////////////////
    //  Below commented code without runtime exception
    ///////////////////////////////////////////////////////////

    /*std::promise<double> Promise;
    int TERM = 1E7;
    auto func = [&]{
        try{
            double ans = CalculatePI(TERM);
            Promise.set_value(ans);
        }
        catch(...)
        {
            Promise.set_exception(std::current_exception());
        }
    };

    std::future<double> Future = Promise.get_future();
 
    std::thread t1(func); 

    std::cout<< std::setprecision(15) << Future.get()<<std::endl; 
    t1.join();
    
    */
    //////////////////////////////////////////////////////
    //  exception handling in multithreading  
    //////////////////////////////////////////////////////
    int TERM = 0;
    std::promise<double> Promise;

    auto func = [&]{    
        try{
            double ans = CalculatePI(TERM);
            Promise.set_value(ans); // set value at future object 
        }
        catch(...)
        {
            Promise.set_exception(std::current_exception());
        }
    };
    
    std::future<double> Future = Promise.get_future();// getting future object from promise object

    std::thread t2(func);// calling func in i different thread 
    
    try
    {
        double ans = Future.get();// get value from future object, set at promise object
        
        std::cout<< std::setprecision(ans)<< std::endl;
    }
    catch(std::exception &e)
    {
        std::cout<<"Runtime Exception: "<< e.what()<< std::endl;
    }
 
    t2.join();

    return 0;
}