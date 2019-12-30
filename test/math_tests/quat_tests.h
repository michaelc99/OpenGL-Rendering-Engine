#ifndef QUAT_TESTS_H
#define QUAT_TESTS_H

#include <iostream>
#include <string>
#include <math/quaternion.h>
#include <test_exception.h>
#include <test_comparison.h>

namespace Tests::QuatTests {

int DoTests();
int TestConstructors();
int TestAccessorsMutators();
int TestAssignmentOperators();
int TestUnaryOperators();
int TestBinaryOperators();
int TestOther();
int TestPerformance();

};

#endif //QUAT_TESTS_H
