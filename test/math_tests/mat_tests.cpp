#include "mat_tests.h"

using namespace Engine;
using namespace Engine::Math;

namespace Tests::MatTests {

int DoTests() {
    int failedCount = 0;
    
    failedCount += TestConstructors();
    failedCount += TestAccessorsMutators();
    failedCount += TestAssignmentOperators();
    failedCount += TestUnaryOperators();
    failedCount += TestBinaryOperators();
    failedCount += TestOther();
    failedCount += TestPerformance();
    
    return failedCount;
}
int TestConstructors() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f_1;
    result << mat5f_1;
    expected << "[0, 0, 0, 0, 0,"
                " 0, 0, 0, 0, 0,"
                " 0, 0, 0, 0, 0,"
                " 0, 0, 0, 0, 0,"
                " 0, 0, 0, 0, 0]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f_2 = Mat<float, 5, 5>(1.0f);
    result << mat5f_2;
    expected << "[1, 0, 0, 0, 0,"
                " 0, 1, 0, 0, 0,"
                " 0, 0, 1, 0, 0,"
                " 0, 0, 0, 1, 0,"
                " 0, 0, 0, 0, 1]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f_3 = Mat<float, 5, 5>(mat5f_2);
    result << mat5f_3;
    expected << mat5f_2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat2f mat2f = createMat2<float>(1.3456f, 4.0f,
                        2.0f, 2.0f);
    result << mat2f;
    expected << "[1.3456, 4, 2, 2]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat3f mat3f = createMat3<float>(1.3456f, 4.0f, -123.0f,
                        2.0f, 2.0f, 2.0f,
                        3.0f, 3.0f, 3.0f);
    result << mat3f;
    expected << "[1.3456, 4, -123, 2, 2, 2, 3, 3, 3]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat4f mat4f_1 = createMat4<float>(createMat3<float>(1.3456f, 4.0f, -123.0f,
                        2.0f, 2.0f, 2.0f,
                        3.0f, 3.0f, 3.0f));
    result << mat4f_1;
    expected << "[1.3456, 4, -123, 0, 2, 2, 2, 0, 3, 3, 3, 0, 0, 0, 0, 1]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat2f mat2f_1 = createMat2<float>(createMat3<float>(1.3456f, 4.0f, -123.0f,
                        2.0f, 2.0f, 2.0f,
                        3.0f, 3.0f, 3.0f));
    result << mat2f_1;
    expected << "[1.3456, 4, 2, 2]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestAccessorsMutators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f(2.0f);
    Mat<float, 5, 5> mat5f_1(3.0f);
    mat5f[1][2] = 3.0f;
    mat5f[4][2] = mat5f_1[0][0];
    mat5f_1 = Mat<float, 5, 5>();
    result << mat5f;
    expected << "[2, 0, 0, 0, 0,"
                " 0, 2, 3, 0, 0,"
                " 0, 0, 2, 0, 0,"
                " 0, 0, 0, 2, 0,"
                " 0, 0, 3, 0, 2]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(2.0f);
    Mat<float, 5, 5> mat5f_2(3.0f);
    mat5f.set(1, 2, 3.0f);
    mat5f.set(4, 2, mat5f_2.at(0, 0));
    mat5f_2 = Mat<float, 5, 5>();
    result << mat5f;
    expected << "[2, 0, 0, 0, 0,"
                " 0, 2, 3, 0, 0,"
                " 0, 0, 2, 0, 0,"
                " 0, 0, 0, 2, 0,"
                " 0, 0, 3, 0, 2]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestAssignmentOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    const char* identity2 =
           "[2, 0, 0, 0, 0,"
           " 0, 2, 0, 0, 0,"
           " 0, 0, 2, 0, 0,"
           " 0, 0, 0, 2, 0,"
           " 0, 0, 0, 0, 2]";
    
    const char* all2s =
            "[2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2]";
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f;
    Mat<float, 5, 5> mat5f_1_1(2.0f);
    mat5f = mat5f_1_1;
    mat5f_1_1 = Mat<float, 5, 5>();
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(1.0f);
    Mat<float, 5, 5> mat5f_2_1(1.0f);
    mat5f += mat5f_2_1;
    mat5f_2_1 = Mat<float, 5, 5>();
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(3.0f);
    Mat<float, 5, 5> mat5f_3_1(1.0f);
    mat5f -= mat5f_3_1;
    mat5f_3_1 = Mat<float, 5, 5>();
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(2.0f);
    Mat<float, 5, 5> mat5f_4_1(2.0f);
    mat5f *= mat5f_4_1;
    mat5f_4_1 = Mat<float, 5, 5>();
    result << mat5f / 2.0f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(1.0f);
    mat5f = 2.0f;
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>();
    mat5f = 1.0f;
    mat5f += 1.0f;
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>();
    mat5f = 4.0f;
    mat5f -= 2.0f;
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(1.0f);
    mat5f *= 2.0f;
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>(4.0f);
    mat5f /= 2.0f;
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestUnaryOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    const char* identity2 =
            "[2, -0, -0, -0, -0,"
            " -0, 2, -0, -0, -0,"
            " -0, -0, 2, -0, -0,"
            " -0, -0, -0, 2, -0,"
            " -0, -0, -0, -0, 2]";
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f;
    Mat<float, 5, 5> mat5f_1(-2.0f);
    mat5f = -mat5f_1;
    result << mat5f;
    expected << identity2;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestBinaryOperators() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    const char* all2s =
            "[2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2,"
            " 2, 2, 2, 2, 2]";
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat<float, 5, 5> mat5f;
    Mat<float, 5, 5> mat5f_1;
    mat5f_1 = -2.0f;
    Mat<float, 5, 5> mat5f_2;
    mat5f_2 = -4.0f;
    Mat<float, 5, 5> mat5f_3;
    mat5f_3 = -3.0f;
    Mat<float, 5, 5> mat5f_4;
    mat5f_4 = 0.5f;
    mat5f = mat5f_1 + mat5f_2;
    mat5f = (0.5f * mat5f) - (2.0f * mat5f_3);
    mat5f = mat5f - 1.0f;
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = mat5f_1 + 1.0f;
    mat5f = (mat5f * 6.0f) + 2.0f;
    mat5f = -mat5f / 2.0f;
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = 1.0f + mat5f_1;
    mat5f = -4.0f - (6.0f * mat5f);
    mat5f = 4.0f + (-mat5f);
    result << mat5f;
    expected << all2s;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f = Mat<float, 5, 5>();
    mat5f += 2.0f;
    Mat<float, 5, 5> mat5f_5 = mat5f + 1.0f;
    result << (mat5f == mat5f_5);
    expected << false;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat5f += 1.0f;
    result << (mat5f != mat5f_5);
    expected << false;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat3f mat3f_1 = createMat3<float>(
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    );
    Mat3f mat3f_2 = createMat3<float>(
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f,
        1.0f, 2.0f, 3.0f
    );
    result << (mat3f_1 * mat3f_2);
    expected << "[21, 27, 33,"
                " 57, 72, 87,"
                " 93, 117, 141]";
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestOther() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat2f mat = createMat2<float>(1.0f, 2.0f, -3.0f, 1.0f);
    Mat2f mat2 = createMat2<float>(1.0f, 2.0000003f, -3.0f, 1.0f);
    result << equalsTol(mat, mat2, 0.0001f);
    expected << true;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    result = std::stringstream();
    expected = std::stringstream();
    mat = createMat2<float>(1.0f, 2.0f, -3.0f, 1.0f);
    mat2 = createMat2<float>(1.0f, 2.03f, -3.0f, 1.0f);
    result << equalsTol(mat, mat2, 0.0001f);
    expected << false;
    CompareResult(ERROR_INFO, expected, result, failedCount);
    
    return failedCount;
}

int TestPerformance() {
    std::stringstream result;
    std::stringstream expected;
    int failedCount = 0;
    
    result = std::stringstream();
    expected = std::stringstream();
    Mat2f mat = createMat2<float>(1.0f, 2.0f, -3.0f, 1.0f);
    Mat2f mat2 = createMat2<float>(1.0f, 2.03f, -3.0f, 1.0f);
    mat * mat * mat * mat2;
    /*result << (Mat2f)(mat * mat2);
    expected << true;
    CompareResult(ERROR_INFO, expected, result, failedCount);*/
    
    return failedCount;
}

};
