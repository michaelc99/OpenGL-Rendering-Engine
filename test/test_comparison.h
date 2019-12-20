#ifndef TEST_COMPARISION_H
#define TEST_COMPARISION_H

#include <iostream>
#include <string>
#include <sstream>
#include "test_exception.h"

void CompareResult(const std::string errorInfo, std::stringstream& expectedStringStream, std::stringstream& resultStringStream);

#endif //TEST_COMPARISION_H