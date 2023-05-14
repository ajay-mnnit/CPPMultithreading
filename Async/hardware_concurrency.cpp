#include <iostream>
#include <thread>

int main()
{
    //static method of thread class returns the concurrency of the machine, this count is not no. of processing units.
    std::cout<<std::thread::hardware_concurrency()<<std::endl;
}