#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>

#include "../smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"

#include "include/message_types.hpp"
#include "include/slave.hpp"


#define PORT     8080
#define MAXLINE 1024

int main(int argc, char** argv)
{
    std::cout<<"This is a example of slave."<<std::endl;

    int slave_id = 0;
    Slave slave("0.0.0.0", slave_id);
    Computable computable = slave.waitForComputable<Computable>();
    printv(computable.str());

    //perform the calculation
    Result result = computable.compute();
    std::cout<<"Computation done, result: "<<result.str()<<std::endl;

    //send the result back to master
    slave.sendResult(result);

    std::cout<<"Result sent out."<<std::endl;

    return 0;
}



