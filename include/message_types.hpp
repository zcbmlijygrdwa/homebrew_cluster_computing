#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

#include <string>
#include <sstream>

class Computable
{
    public:
        int a;
        int b;

        int compute()
        {
            int c = a + 100*b;
            return c;
        }

        std::string str()
        {
            std::stringstream ss;
            ss << "a: "<<a<<", b:"<<b<<std::endl;
            return ss.str(); 
        }
};

class Result
{
    public:
        int c;
};
#endif
