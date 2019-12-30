#include "linear_math_tests.h"

using namespace Engine;
using namespace Engine::Math;

namespace Tests::LinearMathTests {

int DoTests() {
    int failedCount = 0;
    
    failedCount += TestTransforms();
    
    return failedCount;
}    
int TestTransforms() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Vec3f vec3f = Vec3f(5.0f);
    Vec3f vec3f_2 = Vec3f(-3.0f);
    result << translate(vec3f, vec3f_2);
    expected << "[2, 2, 2]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    /*result = std::stringstream();
    expected = std::stringstream();
    vec3f = Vec3f(1.0f, 0.0f, 0.0f);
    Vec3f offsetVec = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f axis = Vec3f(0.0f, 0.0f, 1.0f);
    Vec3f rotatedVec3f = rotateUnitAxis(vec3f, offsetVec, axis, 3.141592653589793238463f / 2.0f);
    result << equalsTol(rotatedVec3f[0], 0.0f, 0.000001f) << ", "
            << equalsTol(rotatedVec3f[1], 1.0f, 0.000001f) << ", "
            << equalsTol(rotatedVec3f[2], 0.0f, 0.000001f);
    expected << "1, 1, 1";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    vec3f = Vec3f(1.0f, 0.0f, 0.0f);
    Vec3f axisVec1 = Vec3f(-1.0f, 0.0f, 0.0f);
    Vec3f axisVec2 = Vec3f(-1.0f, 0.0f, 1.0f);
    rotatedVec3f = rotate(vec3f, axisVec1, axisVec2, 3.141592653589793238463f);
    result << equalsTol(rotatedVec3f[0], -3.0f, 0.000001f) << ", "
            << equalsTol(rotatedVec3f[1], 0.0f, 0.000001f) << ", "
            << equalsTol(rotatedVec3f[2], 0.0f, 0.000001f);
    expected << "1, 1, 1";
    CompareResult(ERROR_INFO, expected, result, failedCount);*/
    
    return failedCount;
}

};
