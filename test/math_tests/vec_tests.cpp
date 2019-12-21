#include "vec_tests.h"

using namespace std;

namespace VecTests {
    int TestConstructors() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f_1;
        result << vec5f_1;
        expected << "[0, 0, 0, 0, 0]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f_2 = Vec<float, 5>(1.0f);
        result << vec5f_2;
        expected << "[1, 1, 1, 1, 1]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f_3 = Vec<float, 5>(vec5f_2);
        result << vec5f_3;
        expected << vec5f_2;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec2f vec2f = Vec2f(1.3456f, 4.0f);
        result << vec2f;
        expected << "[1.3456, 4]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec2f vec2f_1 = Vec2f(2.0f, 2.0f);
        result << Vec3f(vec2f_1);
        expected << "[2, 2, 1]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec3f vec3f = Vec3f(1.3456f, 4.0f, -123.0f);
        result << vec3f;
        expected << "[1.3456, 4, -123]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec3f vec3f_1 = Vec3f(2.0f, 2.0f, 2.0f);
        result << Vec4f(vec3f_1);
        expected << "[2, 2, 2, 1]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec4f vec4f = Vec4f(1.3456f, 4.0f, -123.0f, 4.1324f);
        result << vec4f;
        expected << "[1.3456, 4, -123, 4.1324]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
    
    int TestAccessorsMutators() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f(2.0f);
        Vec<float, 5> vec5f_1(3.0f);
        vec5f[1] = 3.0f;
        vec5f[4] = vec5f_1[0];
        vec5f_1 = Vec<float, 5>();
        result << vec5f;
        expected << "[2, 3, 2, 2, 3]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(2.0f);
        Vec<float, 5> vec5f_2(3.0f);
        vec5f.set(1, 3.0f);
        vec5f.set(4, vec5f_2.at(0));
        vec5f_2 = Vec<float, 5>();
        result << vec5f;
        expected << "[2, 3, 2, 2, 3]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
    
    int TestAssignmentOperators() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f;
        Vec<float, 5> vec5f_1_1(2.0f);
        vec5f = vec5f_1_1;
        vec5f_1_1 = Vec<float, 5>();
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(1.0f);
        Vec<float, 5> vec5f_2_1(1.0f);
        vec5f += vec5f_2_1;
        vec5f_2_1 = Vec<float, 5>();
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(3.0f);
        Vec<float, 5> vec5f_3_1(1.0f);
        vec5f -= vec5f_3_1;
        vec5f_3_1 = Vec<float, 5>();
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(2.0f);
        Vec<float, 5> vec5f_4_1(2.0f);
        vec5f *= vec5f_4_1;
        vec5f_4_1 = Vec<float, 5>();
        result << vec5f;
        expected << "[4, 4, 4, 4, 4]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(1.0f);
        vec5f = 2.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(1.0f);
        vec5f += 1.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(3.0f);
        vec5f -= 1.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(1.0f);
        vec5f *= 2.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(4.0f);
        vec5f /= 2.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
    
    int TestUnaryOperators() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f;
        Vec<float, 5> vec5f_1(-2.0f);
        vec5f = -vec5f_1;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
    
    int TestBinaryOperators() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 5> vec5f;
        Vec<float, 5> vec5f_1(-2.0f);
        Vec<float, 5> vec5f_2(-4.0f);
        Vec<float, 5> vec5f_3(-3.0f);
        Vec<float, 5> vec5f_4(0.5f);
        vec5f = vec5f_1 + vec5f_2;
        vec5f = (vec5f * vec5f_4) - (2.0f * vec5f_3);
        vec5f = vec5f - 1.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = vec5f_1 + 1.0f;
        vec5f = (vec5f * 6.0f) + 2.0f;
        vec5f = -vec5f / 2.0f;
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = 1.0f + vec5f_1;
        vec5f = -4.0f - (6.0f * vec5f);
        vec5f = 4.0f + (-vec5f);
        result << vec5f;
        expected << "[2, 2, 2, 2, 2]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f = Vec<float, 5>(2.0f);
        Vec<float, 5> vec5f_5 = vec5f + 1.0f;
        result << (vec5f < vec5f_5);
        expected << true;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        result << (vec5f > vec5f_5);
        expected << false;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        result << (vec5f <= vec5f_5);
        expected << true;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        result << (vec5f >= vec5f_5);
        expected << false;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        result << (vec5f == vec5f_5);
        expected << false;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec5f += 1.0f;
        result << (vec5f != vec5f_5);
        expected << false;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
    
    int TestOther() {
        stringstream result;
        stringstream expected;
        int failedCount = 0;
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 3> vec3f(3.0f);
        float norm2 = vec3f.norm2();
        result << norm2;
        expected << 27.0f;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        float norm = vec3f.norm();
        result << norm;
        expected << 5.1961524227066318805823390245176f;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec<float, 3> unitVec = vec3f.normalize();
        result << unitVec;
        expected << vec3f / vec3f.norm();
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec3f vec = Vec3f(1.0f, 2.0f, -3.0f);
        Vec3f vec2 = Vec3f(1.0f, 1.0f, 1.0f);
        result << dot(vec, vec2);
        expected << 0.0f;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec = Vec3f(1.0f, 0.0f, 0.0f);
        vec2 = Vec3f(0.0f, 1.0f, 0.0f);
        result << cross(vec, vec2);
        expected << "[0, 0, 1]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        Vec4f vec4_1 = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
        Vec4f vec4_2 = Vec4f(0.0f, 1.0f, 0.0f, 1.0f);
        result << cross(vec4_1, vec4_2);
        expected << "[0, 0, 1, 1]";
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec = Vec3f(1.0f, 2.0f, -3.0f);
        vec2 = Vec3f(1.0f, 2.0000003f, -3.0f);
        result << equalsTol(vec, vec2, 0.0001f);
        expected << true;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        result = stringstream();
        expected = stringstream();
        vec = Vec3f(1.0f, 2.0f, -3.0f);
        vec2 = Vec3f(1.0f, 2.03f, -3.0f);
        result << equalsTol(vec, vec2, 0.0001f);
        expected << false;
        CompareResult(ERROR_INFO, expected, result, failedCount);
        
        return failedCount;
    }
};
