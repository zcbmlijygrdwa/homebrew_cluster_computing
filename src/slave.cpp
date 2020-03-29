#include "slave.hpp"

Slave::Slave()
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

Slave::~Slave()
{
    close(sockfd);
    std::cout<<"Slave socket shutdown."<<std::endl;
}
