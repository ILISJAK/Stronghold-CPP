#include "Exception.h"

Exception::Exception(std::string message) : message(message) {}

const char* Exception::what() const throw()
{
    return message.c_str();
}

InputValidation::InputValidation(std::string message) : Exception(message) {}
InvalidReference::InvalidReference(std::string message) : Exception(message) {}