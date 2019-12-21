#include "linear_math_tests.h"

using namespace std;

namespace LinearMathTests {
    int TestTransforms() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec3f vec3f = Vec3f(5.0f);
        Vec3f vec3f_2 = Vec3f(-3.0f);
        result << translate(vec3f, vec3f_2);
        expected << "[2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result);
        
        return failedCount;
    }
};
