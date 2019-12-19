#ifndef RENDER_EXCEPTION_H
#define RENDER_EXCEPTION_H

#include <exceptions/general_exception.h>

class RenderException : public GeneralException {
    public:
        RenderException(std::string message) : GeneralException(std::string("RenderException:") + message) {}
    private:
};

#endif //RENDER_EXCEPTION_H
