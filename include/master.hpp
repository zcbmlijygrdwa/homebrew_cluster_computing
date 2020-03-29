#ifndef MASTER_HPP
#define MASTER_HPP

#include <string>
#include <sstream>

#define PORT     8080
#define MAXLINE 1024


#include <sys/socket.h>


class Master 
{
    std::string slave_ip;
    int slave_id;
    struct sockaddr_in my_addr;
    struct sockaddr_in to_addr; // address used to send, receive message to/from slave

    //udp
    int sockfd;
    public:
    Master(std::string slave_ip_in, int slave_id_in)
    {
        slave_ip = slave_ip_in;
        slave_id = slave_id_in;


        // create a socket
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        //set up my_addr
        memset((char*)&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port = htons(0);

        //bind to all local addresses
        if ( bind(sockfd, (const struct sockaddr *)&my_addr, sizeof(my_addr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        //set up to_addr
        memset((char*)&to_addr, 0, sizeof(to_addr));
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(PORT);
        if(inet_aton(slave_ip.c_str(), &to_addr.sin_addr)==0)
        {
            perror("inet_aton() failed.");
            exit(EXIT_FAILURE);
        }
    }

    ~Master()
    {
        close(sockfd);
        std::cout<<"Master socket shutdown."<<std::endl;
    }

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
            Result result;
            void * result_ptr = buffer;
            result = *((T_Result*)result_ptr);

            printv(result.c);

            return result;
        }
};
#endif
