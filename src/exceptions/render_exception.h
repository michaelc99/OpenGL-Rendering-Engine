#ifndef RENDER_EXCEPTION_H
#define RENDER_EXCEPTION_H

#include "general_exception.h"

namespace Engine {

class RenderException : public GeneralException {
    public:
        RenderException(std::string message) : GeneralException(std::string("RenderException:") + message) {}
    private:
};

class ShaderException : public RenderException {
    public:
        ShaderException(std::string message) : RenderException(std::string("ShaderException:") + message) {}
    private:
};

};

#endif //RENDER_EXCEPTION_H
