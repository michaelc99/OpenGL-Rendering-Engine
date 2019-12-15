#ifndef RENDER_EXCEPTION_H
#define RENDER_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

class RenderException : std::exception {
    public:
        RenderException(std::string message) : message(message) {}
        const std::string getMessage() const { return message; }
        const char* what() const noexcept { return message.c_str(); }
    private:
       const std::string message;
};

#endif //RENDER_EXCEPTION_H
