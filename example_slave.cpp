#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>

#include "include/message_types.hpp"
#include "include/slave.hpp"

int main(int argc, char** argv)
{
    std::cout<<"This is a example of slave."<<std::endl;

    Slave slave;
    while(true)
    {
        Computable computable = slave.waitForComputable<Computable>();
        std::cout<<"slave got computable: "<<computable.str()<<std::endl;

        //perform the calculation
        Result result = computable.compute();
        std::cout<<"Computation done, result: "<<result.str()<<std::endl;

        //send the result back to master
        slave.sendResult(result);

        std::cout<<"Result sent out."<<std::endl;
    }

    return 0;
}



