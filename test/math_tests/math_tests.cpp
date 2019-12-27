#include <iostream>
#include <string>

#include "vec_tests.h"
#include "mat_tests.h"
#include "quat_tests.h"
#include "linear_math_tests.h"
#include "test_exception.h"

using namespace Engine;

int main() {
    std::cout << "STARTING MATH TESTS." << std::endl;
    int failedCount = 0;
    
    // Vector tests
    try {
        failedCount += VecTests::DoTests();
    }
    catch(GeneralException& e) {
        std::cout << e.getMessage() << std::endl;
        failedCount++;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        failedCount++;
    }
    
    // Matrix tests
    try {
        failedCount += MatTests::DoTests();
    }
    catch(GeneralException& e) {
        std::cout << e.getMessage() << std::endl;
        failedCount++;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        failedCount++;
    }
    
    // Matrix tests
    try {
        failedCount += QuatTests::DoTests();
    }
    catch(GeneralException& e) {
        std::cout << e.getMessage() << std::endl;
        failedCount++;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        failedCount++;
    }
    
    // Linear Math tests
    try {
        failedCount += LinearMathTests::DoTests();
    }
    catch(GeneralException& e) {
        std::cout << e.getMessage() << std::endl;
        failedCount++;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        failedCount++;
    }
    
    if(failedCount > 0) {
        std::cout << "MATH TESTS FAILED:" << std::endl;
        std::cout << "\tFinished math tests with " << failedCount << " failed tests." << std::endl;
    }
    else {
        std::cout << "MATH TESTS PASSED." << std::endl;
    }
    return 0;
}
