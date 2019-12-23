#ifndef LINEAR_MATH_H
#define LINEAR_MATH_H

#include "vector.h"
#include "matrix.h"

template<typename T>
bool equalsTol(const T val1, const T val2, const T tolerance) {
    if(tolerance == (T)0.0) {
        return (val1 == val2);
    }
    if((val1 < val2 - tolerance) || (val1 > val2 + tolerance)) {
        return false;
    }
    return true;
}

template<typename T>
Vec3<T> translate(const Vec3<T>& vecToTranslate, const Vec3<T>& vecToTranslateBy) {
    Mat4<T> translationMat = (Mat4<T>)(Mat4<T>(1.0f).setCol(3, Vec4<T>(vecToTranslateBy)));
    Vec4<T> translatedVec4 = (Vec4<T>)(translationMat * Vec4<T>(vecToTranslate));
    return Vec3<T>(translatedVec4);
}

/*
 * Theta is the counter clockwise angle (in radians) to rotate vecToRotate by.Rotation is performed around the axis defined by
 * rotationAxisUnitVec (form origin) shifted to rotationCenterVec.
 */
template<typename T>
Vec3<T> rotateUnitAxis(const Vec3<T>& vecToRotate, const Vec3<T>& rotationCenterVec, const Vec3<T>& rotationAxisUnitVec, const T theta) {
    Vec4<T> rotatedVec4(vecToRotate);
    
    // Translate
    Mat4<T> translationMat = (Mat4<T>)(Mat4<T>(1.0f).setCol(3, Vec4<T>(-rotationCenterVec)));
    // Quaternion rotation matrix
    T cosTheta = std::cos(theta);
    T sinTheta = std::sin(theta);
    T preCompX = rotationAxisUnitVec[0] * ((T)1.0 - cosTheta);
    T preCompY = rotationAxisUnitVec[1] * ((T)1.0 - cosTheta);
    Mat4<T> rotationMat = Mat4<T>(Mat3<T>(
        rotationAxisUnitVec[0] * preCompX + cosTheta, rotationAxisUnitVec[0] * preCompY - rotationAxisUnitVec[2] * sinTheta, rotationAxisUnitVec[2] * preCompX + rotationAxisUnitVec[1] * sinTheta,
        rotationAxisUnitVec[1] * preCompX + rotationAxisUnitVec[2] * sinTheta, rotationAxisUnitVec[1] * preCompY + cosTheta, rotationAxisUnitVec[2] * preCompY - rotationAxisUnitVec[0] * sinTheta,
        rotationAxisUnitVec[2] * preCompX - rotationAxisUnitVec[1] * sinTheta, rotationAxisUnitVec[2] * preCompY + rotationAxisUnitVec[0] * sinTheta, rotationAxisUnitVec[2] * rotationAxisUnitVec[2] * ((T)1.0 - cosTheta) + cosTheta
    ));
    // Inverse translate
    Mat4<T> invTranslationMat = (Mat4<T>)(Mat4<T>(1.0f).setCol(3, Vec4<T>(rotationCenterVec)));
    
    rotatedVec4 = Vec4<T>(invTranslationMat * (rotationMat * (translationMat * rotatedVec4)));
    return Vec3<T>(rotatedVec4);
}

/*
 * Theta is the counter clockwise angle (in radians) to rotate vecToRotate by. Rotation is performed around the axis defined by
 * the line from rotationAxisVec1 to rotationAxisVec2.
 */
template<typename T>
Vec3<T> rotate(const Vec3<T>& vecToRotate, const Vec3<T>& rotationAxisVec1, const Vec3<T>& rotationAxisVec2, const T theta) {
    Vec4<T> rotatedVec4(vecToRotate);
    
    Vec3f rotationAxisUnitVec((rotationAxisVec2 - rotationAxisVec1).normalize());
    
    // Translate
    Mat4<T> translationMat = (Mat4<T>)(Mat4<T>(1.0f).setCol(3, Vec4<T>(-rotationAxisVec1)));
    // Quaternion rotation matrix
    T cosTheta = std::cos(theta);
    T sinTheta = std::sin(theta);
    T preCompX = rotationAxisUnitVec[0] * ((T)1.0 - cosTheta);
    T preCompY = rotationAxisUnitVec[1] * ((T)1.0 - cosTheta);
    Mat4<T> rotationMat = Mat4<T>(Mat3<T>(
        rotationAxisUnitVec[0] * preCompX + cosTheta, rotationAxisUnitVec[0] * preCompY - rotationAxisUnitVec[2] * sinTheta, rotationAxisUnitVec[2] * preCompX + rotationAxisUnitVec[1] * sinTheta,
        rotationAxisUnitVec[1] * preCompX + rotationAxisUnitVec[2] * sinTheta, rotationAxisUnitVec[1] * preCompY + cosTheta, rotationAxisUnitVec[2] * preCompY - rotationAxisUnitVec[0] * sinTheta,
        rotationAxisUnitVec[2] * preCompX - rotationAxisUnitVec[1] * sinTheta, rotationAxisUnitVec[2] * preCompY + rotationAxisUnitVec[0] * sinTheta, rotationAxisUnitVec[2] * rotationAxisUnitVec[2] * ((T)1.0 - cosTheta) + cosTheta
    ));
    // Inverse translate
    Mat4<T> invTranslationMat = (Mat4<T>)(Mat4<T>(1.0f).setCol(3, Vec4<T>(rotationAxisVec1)));
    
    rotatedVec4 = Vec4<T>(invTranslationMat * (rotationMat * (translationMat * rotatedVec4)));
    return Vec3<T>(rotatedVec4);
}

#endif //LINEAR_MATH_H
