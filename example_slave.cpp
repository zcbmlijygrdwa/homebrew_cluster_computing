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

    // setup udp
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in   servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
                sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printv(sizeof(Computable));

    std::cout<<"Start waiting for receiving data from master..."<<std::endl;
    int n = recvfrom(sockfd, (void *)buffer, MAXLINE,
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
    std::cout<<"Received!"<<std::endl;

    Computable computable;
    void * loaded_computable_ptr = buffer;
    //printf("Client : %s\n", buffer);
    //std::cout<<"Client sent data:"<<std::endl;
    computable = *((Computable*)loaded_computable_ptr);

    printv(computable.str());
    std::cout<<"Excuting computable's compute()"<<std::endl;
    int result = computable.compute();
    printv(result);

    return 0;
}



