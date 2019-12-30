#include "quat_tests.h"

using namespace Engine;
using namespace Engine::Math;

namespace Tests::QuatTests {
    
int DoTests() {
    int failedCount = 0;
    
    failedCount += TestConstructors();
    //failedCount += TestAccessorsMutators();
    //failedCount += TestAssignmentOperators();
    //failedCount += TestUnaryOperators();
    //failedCount += TestBinaryOperators();
    //failedCount += TestOther();
    
    return failedCount;
}

int TestConstructors() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestAccessorsMutators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestAssignmentOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestUnaryOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestBinaryOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestOther() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestPerformance() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Quat<float> quatf;
    result << quatf;
    expected << "[0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

};
