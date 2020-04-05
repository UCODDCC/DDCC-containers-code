#include <matrix-container/handlers/mainHandler.hpp>
#include <matrix-container/utils.hpp>

std::string mainHandler(const std::string& message) {
    if (message == "+<up?>")
        return std::string("+<yes>");

    std::string sub_operation_code = getSubOpCodeFromMessage(message);

    if (sub_operation_code == "multiplication")
        return multiplicationHandler(message);


    return std::string("-undefined opcode<").append(sub_operation_code).append(">");
}
