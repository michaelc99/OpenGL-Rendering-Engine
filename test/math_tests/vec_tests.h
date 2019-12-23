#ifndef VEC_TESTS_H
#define VEC_TESTS_H

#include <iostream>
#include <string>
#include <math/vector.h>
#include <math/matrix.h>
#include <test_exception.h>
#include <test_comparison.h>

namespace VecTests {
    int TestConstructors();
    int TestAccessorsMutators();
    int TestAssignmentOperators();
    int TestUnaryOperators();
    int TestBinaryOperators();
    int TestOther();
};

#endif //VEC_TESTS_H
