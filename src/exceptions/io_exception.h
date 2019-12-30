#ifndef IO_EXCEPTION_H
#define IO_EXCEPTION_H

#include "general_exception.h"

namespace Engine {

class IOException : public GeneralException {
    public:
        IOException(std::string message) : GeneralException(std::string("IOException:") + message) {}
    private:
};

class FileIOException : public IOException {
    public:
        FileIOException(std::string message) : IOException(std::string("FileIOException:") + message) {}
    private:
};

};

namespace Utility {

class XmlFormatException : public Engine::FileIOException {
    public:
        XmlFormatException(std::string message) : Engine::FileIOException(std::string("XmlFormatException:") + message) {}
    private:
};

class ColladaFormatException : public XmlFormatException {
    public:
        ColladaFormatException(std::string message) : XmlFormatException(std::string("ColladaFormatException:") + message) {}
    private:
};

}

#endif //IO_EXCEPTION_H
