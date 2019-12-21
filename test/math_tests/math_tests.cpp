#include <iostream>
#include <string>

#include "vec_tests.h"
#include "mat_tests.h"
#include "linear_math_tests.h"
#include "test_exception.h"

using namespace std;

int main() {
    cout << "STARTING MATH TESTS." << endl;
    int failedCount = 0;
    
    // Vector tests
    try {
        failedCount += VecTests::TestConstructors();
        failedCount += VecTests::TestAccessorsMutators();
        failedCount += VecTests::TestAssignmentOperators();
        failedCount += VecTests::TestUnaryOperators();
        failedCount += VecTests::TestBinaryOperators();
        failedCount += VecTests::TestOther();
    }
    catch(GeneralException& e) {
        cout << e.getMessage() << endl;
        failedCount++;
    }
    catch(exception& e) {
        cout << e.what() << endl;
        failedCount++;
    }
    
    // Matrix tests
    try {
        failedCount += MatTests::TestConstructors();
        failedCount += MatTests::TestAccessorsMutators();
        failedCount += MatTests::TestAssignmentOperators();
        failedCount += MatTests::TestUnaryOperators();
        failedCount += MatTests::TestBinaryOperators();
        failedCount += MatTests::TestOther();
    }
    catch(GeneralException& e) {
        cout << e.getMessage() << endl;
        failedCount++;
    }
    catch(exception& e) {
        cout << e.what() << endl;
        failedCount++;
    }
    
    // Linear Math tests
    try {
        failedCount += LinearMathTests::TestTransforms();
    }
    catch(GeneralException& e) {
        cout << e.getMessage() << endl;
        failedCount++;
    }
    catch(exception& e) {
        cout << e.what() << endl;
        failedCount++;
    }
    
    if(failedCount > 0) {
        cout << "MATH TESTS FAILED:" << endl;
        cout << "\tFinished math tests with " << failedCount << " failed tests." << endl;
    }
    else {
        cout << "MATH TESTS PASSED." << endl;
    }
    return 0;
}
