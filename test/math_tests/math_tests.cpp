#include <iostream>
#include <string>

#include "vec_tests.h"
#include "mat_tests.h"
#include "test_exception.h"

using namespace std;

int main() {
    cout << "STARTING MATH TESTS." << endl;
    int failedCount = 0;
    
    // Vector tests
    try {
        VecTests::TestConstructors();
        VecTests::TestAccessorsMutators();
        VecTests::TestAssignmentOperators();
        VecTests::TestUnaryOperators();
        VecTests::TestBinaryOperators();
        VecTests::TestOther();
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
        MatTests::TestConstructors();
        MatTests::TestAccessorsMutators();
        MatTests::TestAssignmentOperators();
        MatTests::TestUnaryOperators();
        MatTests::TestBinaryOperators();
        MatTests::TestOther();
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
