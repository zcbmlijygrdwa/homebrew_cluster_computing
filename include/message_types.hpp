#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

#include <string>
#include <sstream>

#include <unistd.h>

class Result
{
    public:
        int c;

        std::string str()
        {
            std::stringstream ss;
            ss << "c: "<<c<<std::endl;
            return ss.str(); 
        }
};

class Computable
{
    public:
        int a;
        int b;

        // most importable, how we want to compute....
        Result compute()
        {
            Result result;
            for(int i = 0 ; i < 3 ; i++)
            {
                std::cout<<"Compute stage["<<(i+1)<<"]"<<std::endl;
                result.c = a + 100*b;
                sleep(3);   //simulate slow computation
            }
            return result;
        }

        std::string str()
        {
            std::stringstream ss;
            ss << "a: "<<a<<", b:"<<b<<std::endl;
            return ss.str(); 
        }
};

#endif
