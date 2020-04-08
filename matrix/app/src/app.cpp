#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <matrix-container/kernel/Server.hpp>
#include <matrix-container/handlers/mainHandler.hpp>

int main(int argc, char const *argv[]) {
    FILE * pFile;
    pFile = fopen ("/idle" , "w");
    if (pFile == NULL)
        throw std::runtime_error("cant create /idle file");
    fputs ("1", pFile);
    fclose (pFile);
    fflush(pFile);

    Server server(std::stoi(getenv("PORT")), mainHandler);
    while (true) server.handleNextConnection();
}
