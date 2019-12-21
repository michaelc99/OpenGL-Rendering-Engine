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
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec3f = Vec3f(1.0f, 0.0f, 0.0f);
        Vec3f offsetVec = Vec3f(0.0f, 0.0f, 0.0f);
        Vec3f axis = Vec3f(0.0f, 0.0f, 1.0f);
        Vec3f rotatedVec3f = rotate(vec3f, offsetVec, axis, 3.141592653589793238463f / 2.0f);
        result << equalsTol(rotatedVec3f[0], 0.0f, 0.000001f) << ", "
                << equalsTol(rotatedVec3f[1], 1.0f, 0.000001f) << ", "
                << equalsTol(rotatedVec3f[2], 0.0f, 0.000001f);
        expected << "1, 1, 1";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
};
