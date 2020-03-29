#include "master.hpp"

Master::Master(std::string slave_ip_in, int slave_id_in)
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

Master::~Master()
{
    close(sockfd);
    std::cout<<"Master socket shutdown."<<std::endl;
}
