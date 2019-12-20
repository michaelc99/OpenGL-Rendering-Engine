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

#endif //LINEAR_MATH_H
