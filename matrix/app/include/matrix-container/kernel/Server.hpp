#ifndef _REGIONAL_MASTER_KERNEL_SERVER_
#define _REGIONAL_MASTER_KERNEL_SERVER_

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <fstream>

/**
 * All the handlers must follow this type
 */
typedef std::string (*handlerType)(const std::string&);

struct serverConnection {
    int socket_fd = -1;
    struct sockaddr client_sock_addr{};
    socklen_t len{};
};

class Server {

private:
    int server_fd_;
    sockaddr_in server_sock_addr_{};

public:
    handlerType handler_;
    Server(int portno, handlerType requestHandler);
    void handleNextConnection();
    void exit();
};

#endif
