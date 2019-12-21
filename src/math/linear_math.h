#ifndef LINEAR_MATH_H
#define LINEAR_MATH_H

#include "vec.h"
#include "mat.h"

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
    return Vec3f(translatedVec4);
}

#endif //LINEAR_MATH_H
