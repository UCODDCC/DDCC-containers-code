#include <matrix-container/handlers/mainHandler.hpp>

std::string mainHandler(const std::string& message) {
    if (message == "+<up?>")
        return std::string("+<yes>");

    return std::string("-undefined opcode<to be implement>");
}