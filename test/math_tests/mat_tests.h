#ifndef MAT_TESTS_H
#define MAT_TESTS_H

#include <iostream>
#include <string>
#include <math/vector.h>
#include <math/matrix.h>
#include <test_exception.h>
#include <test_comparison.h>

namespace Tests::MatTests {

int DoTests();
int TestConstructors();
int TestAccessorsMutators();
int TestAssignmentOperators();
int TestUnaryOperators();
int TestBinaryOperators();
int TestOther();
int TestPerformance();

};

#endif //MAT_TESTS_H
