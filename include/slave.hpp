#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>
#include <iostream>
#include <sstream>

#include "message_types.hpp"

#define SERVICE_PORT 8080
#define MAXLINE 1024

class Slave 
{
    struct sockaddr_in my_addr;
    struct sockaddr_in from_addr; // address used to receive/send message from/to master

    //udp
    int sockfd;
    public:
    Slave();

    virtual  ~Slave();

    template<class T_Computable>
        T_Computable waitForComputable()
        {    
            char buffer[MAXLINE];
            socklen_t len;
            std::cout<<"Start waiting for receiving computable from master..."<<std::endl;
            printf("waiting on port %d\n", SERVICE_PORT);
            int n = recvfrom(sockfd, (void *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &from_addr,
                    &len);
            std::cout<<"Slave received computable from master!"<<std::endl;

            T_Computable computable;
            void * computable_ptr = buffer;
            computable = *((T_Computable*)computable_ptr);

            return computable;
        }

    template<class T_Result>
        void sendResult(T_Result result)
        {
            char buffer[MAXLINE];
            socklen_t len;
            void * loaded_result_ptr = (void *) calloc(1,sizeof(T_Result));
            memcpy(loaded_result_ptr, &result,sizeof(T_Result));
            int send_result = sendto(sockfd, (const void *)loaded_result_ptr, sizeof(T_Result), MSG_CONFIRM, (const struct sockaddr *) &from_addr, sizeof(from_addr));
            std::cout<<"send_result = "<<send_result<<std::endl;
            delete(loaded_result_ptr);
        }
};
#endif
