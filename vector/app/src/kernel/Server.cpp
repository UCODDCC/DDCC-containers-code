#include <vector-container/kernel/Server.hpp>


Server::Server(int portno, handlerType requestHandler) {
    int retval;
    bzero((char*)&this->server_sock_addr_, sizeof(server_sock_addr_));
    this->server_sock_addr_.sin_port = htons(portno);
    this->server_sock_addr_.sin_family = AF_INET;
    this->server_sock_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    this->handler_ = requestHandler;

    this->server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd_ < 0)
        throw std::runtime_error("error: can not open socket");

    retval = bind(
            this->server_fd_,
            (const sockaddr*) &this->server_sock_addr_,
            sizeof(this->server_sock_addr_)
    );
    if (retval < 0)
        throw std::runtime_error("error: can not bind to socket");

    retval = listen(this->server_fd_, 5); // 5 is the maximum size permitted by most systems
    if (retval == -1)
        throw std::runtime_error("error: can not set socket to listen on port");
}


void Server::handleNextConnection() {
    FILE * pFile;
    pFile = fopen ("./idle" , "w");
    if (pFile == NULL)
        throw std::runtime_error("cant create /idle file");
    fputs ("1", pFile);
    fclose (pFile);
    fflush(pFile);

    serverConnection client;
    std::string payload;
    char buffer[1024];
    client.socket_fd = accept(
            this->server_fd_,
            (sockaddr*) &client.client_sock_addr,
            &client.len
    );
    if (client.socket_fd == -1) {
        return;
    }
    do{
        bzero(buffer, 1024);
        read(client.socket_fd, buffer, 1024);
        payload.append(buffer);
    } while (buffer[strlen(buffer)-1]!='>');

    std::string response = this->handler_(payload);

    send(
            client.socket_fd,
            response.c_str(),
            strlen(response.c_str()),
            0
    );
    close(client.socket_fd);

    pFile = fopen ("/idle" , "w");
    if (pFile == NULL)
        throw std::runtime_error("cant create /idle file");
    fputs ("1", pFile);
    fclose (pFile);
    fflush(pFile);
}

void Server::exit() {
    close(this->server_fd_);
    printf("server closed\n");
}