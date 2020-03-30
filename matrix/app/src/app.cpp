#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <matrix-container/kernel/Server.hpp>
#include <matrix-container/handlers/mainHandler.hpp>

int main(int argc, char const *argv[])
{
    /*if (getenv ("PORT") == NULL)
        throw std::runtime_error("env PORT is not set!");
    if (getenv ("RESOURCE") == NULL)
        throw std::runtime_error("env RESOURCE is not set!");
    if (std::string(getenv ("RESOURCE")) != "matrix")
        throw std::runtime_error("env RESOURCE is not 'matrix'!");*/


    FILE * pFile;
    pFile = fopen ("/idle" , "w");
    if (pFile == NULL)
        throw std::runtime_error("cant create /idle file");
    fputs ("1", pFile);
    fclose (pFile);
    fflush(pFile);


    Server server(std::stoi("7007"), mainHandler);
    while (true) server.handleNextConnection();
}
