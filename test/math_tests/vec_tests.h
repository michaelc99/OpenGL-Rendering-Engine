#ifndef VEC_TESTS_H
#define VEC_TESTS_H

#include <iostream>
#include <string>
#include <math/vec.h>
#include <math/mat.h>
#include <test_exception.h>
#include <test_comparison.h>

namespace VecTests {
    void TestConstructors();
    void TestAccessorsMutators();
    void TestAssignmentOperators();
    void TestUnaryOperators();
    void TestBinaryOperators();
    void TestOther();
};

#endif //VEC_TESTS_H
