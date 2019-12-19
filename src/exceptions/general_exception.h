#ifndef GENERAL_EXCEPTION_H
#define GENERAL_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define ERROR_INFO std::string(__FILE__) + " at line: " + std::to_string(__LINE__)

class GeneralException : std::exception {
    public:
        GeneralException(std::string message) : message(message) {}
        GeneralException() : message("ERROR: Unspecified GeneralException occurred.") {}
        const std::string getMessage() const { return message; }
        const char* what() const noexcept { return message.c_str(); }
    private:
       const std::string message;
};

#endif //GENERAL_EXCEPTION_H
