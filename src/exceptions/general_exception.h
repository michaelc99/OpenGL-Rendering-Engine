#ifndef GENERAL_EXCEPTION_H
#define GENERAL_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

class GeneralException;

#define ERROR_INFO std::string(__FILE__) + " at line " + std::to_string(__LINE__)
#ifdef _DEBUG
    #define ADD_ERROR_INFO(statement) try { statement; } catch(GeneralException& e) { throw GeneralException(ERROR_INFO + ":" + e.getMessage()); }
#else
    #define ADD_ERROR_INFO(statement) statement;
#endif

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
