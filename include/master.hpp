#ifndef MASTER_HPP
#define MASTER_HPP

#include <string>
#include <sstream>

#define PORT     8080
#define MAXLINE 1024


#include <sys/socket.h>


class Master 
{
    std::string ip;
    int slave_id;
            
            //udp
            int sockfd;
    public:
    Master(std::string ip_in, int slave_id_in)
    {
        ip = ip_in;
        slave_id = slave_id_in;
    }

    template<class T_Computable>
        void sendData(T_Computable computable)
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
            servaddr.sin_port = htons(PORT);
            servaddr.sin_addr.s_addr = INADDR_ANY;

            socklen_t len;

            void * loaded_computable_ptr = (void *) calloc(1,sizeof(T_Computable));

            memcpy(loaded_computable_ptr, &computable,sizeof(T_Computable));

            sendto(sockfd, (const void *)loaded_computable_ptr, sizeof(T_Computable), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            delete(loaded_computable_ptr);
        }

    template<class T_Result>
        T_Result waitForResult()
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
            servaddr.sin_port = htons(8081);
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
            std::cout<<"Master result received!"<<std::endl;

            Result result;
            void * result_ptr = buffer;
            //printf("Client : %s\n", buffer);
            //std::cout<<"Client sent data:"<<std::endl;
            result = *((T_Result*)result_ptr);

            printv(result.c);

            return result;
        }
};
#endif
