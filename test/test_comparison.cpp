#include "test_comparison.h"

using namespace Engine;

namespace Tests {

void CompareResult(const std::string errorInfo, std::stringstream& expectedStringStream, std::stringstream& resultStringStream) {
    std::string word;
    std::string expectedString;
    while(expectedStringStream >> word) {
        expectedString += word;
    }
    std::string resultString;
    while(resultStringStream >> word) {
        resultString += word;
    }
    if(resultString != expectedString) {
        throw TestFailedException(errorInfo, expectedString, resultString);
    }
}

void CompareResult(const std::string errorInfo, std::stringstream& expectedStringStream, std::stringstream& resultStringStream, int& failedCount) {
    std::string word;
    std::string expectedString;
    while(expectedStringStream >> word) {
        expectedString += word;
    }
    std::string resultString;
    while(resultStringStream >> word) {
        resultString += word;
    }
    if(resultString != expectedString) {
        std::cout << errorInfo + ":Test failed:\n\tExpected: " + expectedString + "\n\tResult: " + resultString << std::endl;
        failedCount++;
    }
}

}
