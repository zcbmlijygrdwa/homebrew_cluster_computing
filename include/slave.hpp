#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <string>
#include <sstream>

#define SERVICE_PORT 8080
#define MAXLINE 1024


#include <sys/socket.h>


class Slave 
{
    struct sockaddr_in my_addr;
    struct sockaddr_in from_addr; // address used to receive/send message from/to master

    //udp
    int sockfd;
    public:
    Slave()
    {
        // create a socket
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        //set up my_addr
        memset((char*)&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port = htons(SERVICE_PORT);

        //bind to all local addresses
        if ( bind(sockfd, (const struct sockaddr *)&my_addr, sizeof(my_addr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    ~Slave()
    {
        close(sockfd);
        std::cout<<"Slave socket shutdown."<<std::endl;
    }

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

            Computable computable;
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
            sendto(sockfd, (const void *)loaded_result_ptr, sizeof(T_Result), MSG_CONFIRM, (const struct sockaddr *) &from_addr, sizeof(from_addr));
            delete(loaded_result_ptr);
        }
};
#endif
