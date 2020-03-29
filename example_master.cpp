#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>

#include "../smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"

#include "include/message_types.hpp"

#define PORT     8080
#define MAXLINE 1024

int main(int argc, char** argv)
{
    std::cout<<"This is a example of master"<<std::endl;

    //int slace_id = 0;
    //Slave slave("127.0.0.1", slave_id);
    //slave.sendData(loaded_computable_object);
    //Result result = slave.computeForResult();




    // setup udp
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in   servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;

    void * loaded_computable_ptr = (void *) calloc(1,sizeof(Computable));

    Computable computable;
    computable.a = 1;
    computable.b = 1;

    memcpy(loaded_computable_ptr, &computable,sizeof(Computable));

    sendto(sockfd, (const void *)loaded_computable_ptr, sizeof(Computable), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    delete(loaded_computable_ptr);

    //printv(result.toString());

    return 0;
}



