#include <stdlib.h>
#include <stdexcept>
#include <matrix-container/kernel/Server.hpp>
#include <matrix-container/handlers/mainHandler.hpp>

int main(int argc, char const *argv[])
{
    if (getenv ("PORT") == NULL)
        throw std::runtime_error("env PORT is not set!");
    if (getenv ("RESOURCE") == NULL)
        throw std::runtime_error("env RESOURCE is not set!");
    if (std::string(getenv ("RESOURCE")) != "matrix")
        throw std::runtime_error("env RESOURCE is not 'matrix'!");
    Server server(std::stoi(getenv ("PORT")), mainHandler);
    while (true) server.handleNextConnection();
}
