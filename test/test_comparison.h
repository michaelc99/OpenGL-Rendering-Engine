#ifndef TEST_COMPARISION_H
#define TEST_COMPARISION_H

#include <iostream>
#include <string>
#include <sstream>
#include "test_exception.h"

namespace Tests {

void CompareResult(const std::string errorInfo, std::stringstream& expectedStringStream, std::stringstream& resultStringStream);
void CompareResult(const std::string errorInfo, std::stringstream& expectedStringStream, std::stringstream& resultStringStream, int& failedCount);

}
#endif //TEST_COMPARISION_H
