#ifndef MASTER_HPP
#define MASTER_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>
#include <iostream>
#include <sstream>

#include "message_types.hpp"

#define PORT     8080
#define MAXLINE 1024

class Master 
{
    std::string slave_ip;
    int slave_id;
    struct sockaddr_in my_addr;
    struct sockaddr_in to_addr; // address used to send, receive message to/from slave

    //udp
    int sockfd;
    public:
    Master(std::string slave_ip_in, int slave_id_in);

    ~Master();

    template<class T_Computable>
        void sendData(T_Computable computable)
        {
            // setup udp
            char buffer[MAXLINE];
            void * loaded_computable_ptr = (void *) calloc(1,sizeof(T_Computable));
            memcpy(loaded_computable_ptr, &computable,sizeof(T_Computable));
            sendto(sockfd, (const void *)loaded_computable_ptr, sizeof(T_Computable), MSG_CONFIRM, (const struct sockaddr *) &to_addr, sizeof(to_addr));
            delete(loaded_computable_ptr);

        }

    template<class T_Result>
        T_Result waitForResult()
        {
            // setup udp
            char buffer[MAXLINE];
            socklen_t len;
            std::cout<<"Start waiting for receiving result from slave..."<<std::endl;
            int n = recvfrom(sockfd, (void *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &to_addr,
                    &len);
            std::cout<<"Master result received!"<<std::endl;
            T_Result result;
            void * result_ptr = buffer;
            result = *((T_Result*)result_ptr);

            std::cout<<"result: "<<result.str()<<std::endl;

            return result;
        }
};
#endif
