#ifndef _DDCD_UTILS_
#define _DDCD_UTILS_

#include <string>

/**
 * string splitter
 * @param message the full string
 * @param opening_delimiter
 * @param close_delimiter
 * @return substring between delimiter characters, not including them
 */
std::string getDelimitedMessage(const std::string& message, char opening_delimiter, char close_delimiter);

/**
 * series of parsers for the message
 * @param message full client or server message
 * @return interested part of the message
 */
std::string getPayloadFromMessage(const std::string& message);
std::string getDataFromMessage(const std::string& message);
std::string getOperationFromMessage(const std::string& message);
std::string getErrorFromMessage(const std::string& message);
std::string getOpCodeFromMessage(const std::string& message);
std::string getSubOpCodeFromMessage(const std::string& message);
std::string getMetaFromMessage(const std::string& message);
std::string popCSV(std::string* data);

#endif