#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <string>
#include <sstream>

#define PORT     8080
#define MAXLINE 1024


#include <sys/socket.h>


class Slave 
{
    std::string ip;
    int slave_id;

    //udp
    int sockfd;
    public:
    Slave(std::string ip_in, int slave_id_in)
    {
        ip = ip_in;
        slave_id = slave_id_in;
    }

    template<class T_Computable>
        T_Computable waitForComputable()
        {
            // setup udp
            int sockfd;
            char buffer[MAXLINE];
            struct sockaddr_in servaddr, cliaddr;

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

            std::cout<<"Start waiting for receiving result from slave..."<<std::endl;
            int n = recvfrom(sockfd, (void *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &len);
            std::cout<<"Slave result received!"<<std::endl;

            Computable computable;
            void * computable_ptr = buffer;
            //printf("Client : %s\n", buffer);
            //std::cout<<"Client sent data:"<<std::endl;
            computable = *((T_Computable*)computable_ptr);

            return computable;
        }

    template<class T_Result>
        void sendResult(T_Result result)
        {
            // setup udp
            char buffer[MAXLINE];
            struct sockaddr_in   servaddr;

            // Creating socket file descriptor
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }

            memset(&servaddr, 0, sizeof(servaddr));

            // Filling server information
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(8081);
            servaddr.sin_addr.s_addr = INADDR_ANY;

            socklen_t len;

            void * loaded_result_ptr = (void *) calloc(1,sizeof(T_Result));

            memcpy(loaded_result_ptr, &result,sizeof(T_Result));

            sendto(sockfd, (const void *)loaded_result_ptr, sizeof(T_Result), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            delete(loaded_result_ptr);
        }
};
#endif
