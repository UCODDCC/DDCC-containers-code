#include <matrix-container/handlers/mainHandler.hpp>
#include <matrix-container/utils.hpp>

std::string mainHandler(const std::string& message) {
    if (message == "+<up?>")
        return std::string("+<yes>");

    std::string operation_code = getOpCodeFromMessage(message);
    std::string sub_operation_code = getSubOpCodeFromMessage(message);

    if (operation_code != "matrix")
        return std::string("-undefined opcode<this is a matrix container, you send a ").append(operation_code).append(" operation>");

    if (sub_operation_code == "multiplication")
        return multiplicationHandler(message);

    return std::string("-undefined sub-opcode<you send a ").append(sub_operation_code).append(" suboperation>");
}
