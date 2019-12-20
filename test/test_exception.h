#ifndef TEST_EXCEPTION_H
#define TEST_EXCEPTION_H

#include <exceptions/general_exception.h>

class TestFailedException : public GeneralException {
    public:
        TestFailedException(std::string message) : GeneralException(std::string("TestFailedException:") + message) {}
        TestFailedException(std::string message, std::string expected, std::string result)
            : TestFailedException(message + "\nExpected: " + expected + "\nResult: " + result) {}
    private:
};

#endif //TEST_EXCEPTION_H
