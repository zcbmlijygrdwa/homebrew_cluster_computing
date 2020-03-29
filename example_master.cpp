#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>

#include "../smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"

#include "include/message_types.hpp"
#include "include/master.hpp"

#define PORT     8080
#define MAXLINE 1024

int main(int argc, char** argv)
{
    std::cout<<"This is a example of master"<<std::endl;

    Computable computable;
    computable.a = 1;
    computable.b = 1;

    int master_id = 0;
    Master master("127.0.0.1", master_id);
    master.sendData<Computable>(computable);
    Result result = master.waitForResult<Result>();

    return 0;
}



