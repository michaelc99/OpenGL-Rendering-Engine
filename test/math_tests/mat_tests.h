#ifndef MAT_TESTS_H
#define MAT_TESTS_H

#include <iostream>
#include <string>
#include <math/vec.h>
#include <math/mat.h>
#include <test_exception.h>
#include <test_comparison.h>

namespace MatTests {
    void TestConstructors();
    void TestAccessorsMutators();
    void TestAssignmentOperators();
    void TestUnaryOperators();
    void TestBinaryOperators();
    void TestOther();
};

#endif //MAT_TESTS_H
