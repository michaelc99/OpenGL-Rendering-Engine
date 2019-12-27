#ifndef MAT_H
#define MAT_H

#include <exceptions/math_exception.h>
#include "vector.h"
#include <cmath>
#include <cassert>
#include <iostream>

namespace Engine::Math {

template<typename T, size_t ROWS, size_t COLS>
class Mat;
template<typename T, size_t ROWS, size_t COLS>
bool equalsTol(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2, const T tolerance);
template<typename T, size_t ROWS, size_t COLS>
bool operator==(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator!=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
std::ostream& operator<<(std::ostream& out, const Mat<T, ROWS, COLS>& mat);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const Mat<T, ROWS, COLS>& mat, const T val);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const T val, const Mat<T, ROWS, COLS>& mat);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const Mat<T, ROWS, COLS>& mat, const T val);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const T val, const Mat<T, ROWS, COLS>& mat);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const Mat<T, ROWS, COLS>& mat, const T val);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const T val, const Mat<T, ROWS, COLS>& mat);
template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator/(const Mat<T, ROWS, COLS>& mat, const T val);
template<typename T, size_t ROWS, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, ROWS>& vec, const Mat<T, ROWS, COLS>& mat);
template<typename T, size_t ROWS, size_t COLS>
Vec<T, ROWS> operator*(const Mat<T, ROWS, COLS>& mat, const Vec<T, COLS>& vec);

/*
 * Matrix
 * 
 * Layout as row major (also in memory):
 *      [ x1, y1, z1, ...]
 *      [ x2, y2, z2, ...]
 *      [ x3, y3, z3, ...]
 *      [      ...       ]
 *      [      ...       ]
 *      [      ...       ]
 * 
 */
template<typename T, size_t ROWS, size_t COLS>
class Mat {
    public:
        Mat() {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = Vec<T, COLS>();
            }
        }
        
        /*
         * Constructor for diagonal matrix with diagonal elements of diagVal.
         */
        Mat(const T diagVal) : Mat() {
            size_t rank = std::min(ROWS, COLS);
            for(size_t r = 0; r < rank; r++) {
                dataVecs[r][r] = diagVal;
            }
        }
        
        Mat(const Mat<T, ROWS, COLS>& mat) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = mat[r];
            }
        }
        
        inline T at(const size_t row, const size_t col) const {
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
            assert(col >= 0 && col < COLS);
#endif
            return dataVecs[row][col];
        }
        
        inline Mat<T, ROWS, COLS>& set(const size_t row, const size_t col, const T val) {
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
            assert(col >= 0 && col < COLS);
#endif
            dataVecs[row][col] = val;
            return (*this);
        }
        
        inline Vec<T, COLS> getRow(const size_t row) const {
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
#endif
            return dataVecs[row];
        }
        
        inline Mat<T, ROWS, COLS>& setRow(const size_t row, const Vec<T, COLS> rowVec) {
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
#endif
            dataVecs[row] = rowVec;
            return (*this);
        }
        
        Vec<T, ROWS> getCol(const size_t col) const {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            Vec<T, ROWS> colVec;
            for(size_t r = 0; r < ROWS; r++) {
                colVec[r] = dataVecs[r][col];
            }
            return colVec;
        }
        
        Mat<T, ROWS, COLS>& setCol(const size_t col, const Vec<T, ROWS> colVec) {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r][col] = colVec[r];
            }
            return (*this);
        }
        
        inline Vec<T, COLS> operator[](const size_t row) const { // Row major so [] operator returns a row
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
#endif
            return dataVecs[row];
        }
        
        inline Vec<T, COLS>& operator[](const size_t row) { // Row major so [] operator returns a row
#ifdef _DEBUG
            assert(row >= 0 && row < ROWS);
#endif
            return dataVecs[row];
        }
        
        Mat<T, ROWS, COLS>& operator=(const Mat<T, ROWS, COLS>& mat) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = mat[r];
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator+=(const Mat<T, ROWS, COLS>& mat) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] += mat[r];
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator-=(const Mat<T, ROWS, COLS>& mat) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] -= mat[r];
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator*=(const Mat<T, ROWS, COLS>& mat) {
            Mat<T, ROWS, COLS> oldMat = (*this);
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = oldMat[r] * mat;
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator=(const T val) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = val;
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator+=(const T val) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] += val;
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator-=(const T val) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] -= val;
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator*=(const T val) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] *= val;
            }
            return (*this);
        }
        
        Mat<T, ROWS, COLS>& operator/=(const T val) {
            Mat<T, ROWS, COLS> newMat;
            if(val == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            for(size_t r = 0; r < ROWS; r++) {
                newMat[r] = dataVecs[r] / val;
            }
            (*this) = newMat;
            return (*this);
        }
        
        Mat<T, ROWS, COLS> operator-() const {
            Mat<T, ROWS, COLS> newMat;
            for(size_t r = 0; r < ROWS; r++) {
                newMat[r] = -(dataVecs[r]);
            }
            return newMat;
        }
        
        friend bool equalsTol <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2, const T tolerance);
        friend bool operator== <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend bool operator!= <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend std::ostream& operator<< <T, COLS>(std::ostream& out, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> (Engine::Math::operator- <T, ROWS, COLS>)(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> (Engine::Math::operator- <T, ROWS, COLS>)(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> (Engine::Math::operator- <T, ROWS, COLS>)(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator/ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Vec<T, COLS> operator* <T, ROWS, COLS>(const Vec<T, COLS>& vec, const Mat<T, ROWS, COLS>& mat);
        friend Vec<T, ROWS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const Vec<T, ROWS>& vec);
    protected:
        Vec<T, COLS> dataVecs[ROWS];
};

/*
 * 
 */
template<typename T, size_t ROWS, size_t COLS>
bool equalsTol(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2, const T tolerance) {
    if(tolerance == (T)0.0) {
        return (mat1 == mat2);
    }
    for(size_t r = 0; r < ROWS; r++) {
        for(size_t c = 0; c < COLS; c++) {
            if((mat1[r][c] < mat2[r][c] - tolerance) || (mat1[r][c] > mat2[r][c] + tolerance)) {
                return false;
            }
        }
    }
    return true;
}

template<typename T, size_t ROWS, size_t COLS>
bool operator==(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    for(size_t r = 0; r < ROWS; r++) {
        if(mat1[r] != mat2[r]) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t ROWS, size_t COLS>
bool operator!=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    return !(mat1 == mat2);
}

template<typename T, size_t ROWS, size_t COLS>
std::ostream& operator<<(std::ostream& out, const Mat<T, ROWS, COLS>& mat) {
    out << "[";
    for(size_t r = 0; r < ROWS; r++) {
        for(size_t c = 0; c < COLS; c++) {
            out << mat[r][c] << ((r < COLS - 1 || c < COLS - 1) ? ", " : "");
        }
    }
    return out << "]";
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat1[r] + mat2[r];
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat1[r] - mat2[r];
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat1[r] * mat2;
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const Mat<T, ROWS, COLS>& mat, const T val) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat[r] + val;
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator+(const T val, const Mat<T, ROWS, COLS>& mat) {
    return mat + val;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const Mat<T, ROWS, COLS>& mat, const T val) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat[r] - val;
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator-(const T val, const Mat<T, ROWS, COLS>& mat) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = val - mat[r];
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const Mat<T, ROWS, COLS>& mat, const T val) {
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat[r] * val;
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator*(const T val, const Mat<T, ROWS, COLS>& mat) {
    return mat * val;
}

template<typename T, size_t ROWS, size_t COLS>
Mat<T, ROWS, COLS> operator/(const Mat<T, ROWS, COLS>& mat, const T val) {
    if(val == (T)0.0) {
        throw DivideByZeroException(ERROR_INFO);
    }
    Mat<T, ROWS, COLS> newMat;
    for(size_t r = 0; r < ROWS; r++) {
        newMat[r] = mat[r] / val;
    }
    return newMat;
}

template<typename T, size_t ROWS, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, ROWS>& vec, const Mat<T, ROWS, COLS>& mat) {
    Vec<T, COLS> newVec;
    for(size_t r = 0; r < ROWS; r++) {
        newVec += mat[r] * vec[r];
    }
    return newVec;
}

template<typename T, size_t ROWS, size_t COLS>
Vec<T, ROWS> operator*(const Mat<T, ROWS, COLS>& mat, const Vec<T, COLS>& vec) {
    Vec<T, ROWS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        for(size_t r = 0; r < ROWS; r++) {
            newVec[r] += mat[r][c] * vec[c];
        }
    }
    return newVec;
}

// Explicitly sized typedefs
template<typename T>
using Mat2 = Mat<T, 2, 2>;

template<typename T>
using Mat3 = Mat<T, 3, 3>;

template<typename T>
using Mat3x2 = Mat<T, 3, 2>;

template<typename T>
using Mat2x3 = Mat<T, 2, 3>;

template<typename T>
using Mat4 = Mat<T, 4, 4>;

template<typename T>
using Mat4x3 = Mat<T, 4, 3>;

template<typename T>
using Mat4x2 = Mat<T, 4, 2>;

template<typename T>
using Mat3x4 = Mat<T, 3, 4>;

template<typename T>
using Mat2x4 = Mat<T, 2, 4>;

// Factory constructors
template<typename T>
Mat2<T> createMat2(T x1, T y1,
                   T x2, T y2) {
    Mat2<T> newMat;
    newMat[0] = createVec2<T>(x1, y1);
    newMat[1] = createVec2<T>(x2, y2);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat2<T> createMat2(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat2<T> newMat;
    for(size_t r = 0; r < 2; r++) {
        for(size_t c = 0; c < 2; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat3<T> createMat3(T x1, T y1, T z1,
                   T x2, T y2, T z2,
                   T x3, T y3, T z3) {
    Mat3<T> newMat;
    newMat[0] = createVec3<T>(x1, y1, z1);
    newMat[1] = createVec3<T>(x2, y2, z2);
    newMat[2] = createVec3<T>(x3, y3, z3);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat3<T> createMat3(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat3<T> newMat;
    for(size_t r = 0; r < 3; r++) {
        for(size_t c = 0; c < 3; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat3x2<T> createMat3x2(T x1, T y1,
                       T x2, T y2,
                       T x3, T y3) {
    Mat3x2<T> newMat;
    newMat[0] = createVec2<T>(x1, y1);
    newMat[1] = createVec2<T>(x2, y2);
    newMat[2] = createVec2<T>(x3, y3);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat3x2<T> createMat3x2(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat3x2<T> newMat;
    for(size_t r = 0; r < 3; r++) {
        for(size_t c = 0; c < 2; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat2x3<T> createMat2x3(T x1, T y1, T z1,
                       T x2, T y2, T z2) {
    Mat2x3<T> newMat;
    newMat[0] = createVec3<T>(x1, y1, z1);
    newMat[1] = createVec3<T>(x2, y2, z2);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat2x3<T> createMat2x3(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat2x3<T> newMat;
    for(size_t r = 0; r < 2; r++) {
        for(size_t c = 0; c < 3; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat4<T> createMat4(T x1, T y1, T z1, T w1,
                   T x2, T y2, T z2, T w2,
                   T x3, T y3, T z3, T w3,
                   T x4, T y4, T z4, T w4) {
    Mat4<T> newMat;
    newMat[0] = createVec4<T>(x1, y1, z1, w1);
    newMat[1] = createVec4<T>(x2, y2, z2, w2);
    newMat[2] = createVec4<T>(x3, y3, z3, w3);
    newMat[3] = createVec4<T>(x4, y4, z4, w4);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat4<T> createMat4(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat4<T> newMat;
    for(size_t r = 0; r < 4; r++) {
        for(size_t c = 0; c < 4; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat4x3<T> createMat4x3(T x1, T y1, T z1,
                       T x2, T y2, T z2,
                       T x3, T y3, T z3,
                       T x4, T y4, T z4) {
    Mat4<T> newMat;    
    newMat[0] = createVec3<T>(x1, y1, z1);
    newMat[1] = createVec3<T>(x2, y2, z2);
    newMat[2] = createVec3<T>(x3, y3, z3);
    newMat[3] = createVec3<T>(x4, y4, z4);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat4x3<T> createMat4x3(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat4x3<T> newMat;
    for(size_t r = 0; r < 4; r++) {
        for(size_t c = 0; c < 3; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat4x2<T> createMat4x2(T x1, T y1,
                       T x2, T y2,
                       T x3, T y3,
                       T x4, T y4) {
    Mat4x2<T> newMat;
    newMat[0] = createVec2<T>(x1, y1);
    newMat[1] = createVec2<T>(x2, y2);
    newMat[2] = createVec2<T>(x3, y3);
    newMat[3] = createVec2<T>(x4, y4);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat4x2<T> createMat4x2(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat4x2<T> newMat;
    for(size_t r = 0; r < 4; r++) {
        for(size_t c = 0; c < 2; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat3x4<T> createMat3x4(T x1, T y1, T z1, T w1,
                       T x2, T y2, T z2, T w2,
                       T x3, T y3, T z3, T w3) {
    Mat3x4<T> newMat;
    newMat[0] = createVec4<T>(x1, y1, z1, w1);
    newMat[1] = createVec4<T>(x2, y2, z2, w2);
    newMat[2] = createVec4<T>(x3, y3, z3, w3);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat3x4<T> createMat3x4(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat3x4<T> newMat;
    for(size_t r = 0; r < 3; r++) {
        for(size_t c = 0; c < 4; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

template<typename T>
Mat2x4<T> createMat2x4(T x1, T y1, T z1, T w1,
                       T x2, T y2, T z2, T w2) {
    Mat2x4<T> newMat;
    newMat[0] = createVec4<T>(x1, y1, z1, w1);
    newMat[1] = createVec4<T>(x2, y2, z2, w2);
    return newMat;
}

template<typename T, size_t OTHER_ROWS, size_t OTHER_COLS>
Mat2x4<T> createMat2x4(const Mat<T, OTHER_ROWS, OTHER_COLS>& mat) {
    Mat2x4<T> newMat;
    for(size_t r = 0; r < 2; r++) {
        for(size_t c = 0; c < 4; c++) {
            if(r < OTHER_ROWS && c < OTHER_COLS) {
                newMat[r][c] = mat[r][c];
            }
            else {
                newMat[r][c] = (T)(r == c);
            }
        }
    }
    return newMat;
}

// Typedefs
typedef Mat2<float> Mat2f;
typedef Mat2<double> Mat2d;
typedef Mat2<int> Mat2i;

typedef Mat3<float> Mat3f;
typedef Mat3x2<float> Mat3x2f;
typedef Mat2x3<float> Mat2x3f;
typedef Mat3<double> Mat3d;
typedef Mat3x2<double> Mat3x2d;
typedef Mat2x3<double> Mat2x3d;
typedef Mat3<int> Mat3i;
typedef Mat3x2<float> Mat3x2i;
typedef Mat2x3<float> Mat2x3i;

typedef Mat4<float> Mat4f;
typedef Mat4x3<float> Mat4x3f;
typedef Mat4x2<float> Mat4x2f;
typedef Mat3x4<float> Mat3x4f;
typedef Mat2x4<float> Mat2x4f;
typedef Mat4<double> Mat4d;
typedef Mat4x3<double> Mat4x3d;
typedef Mat4x2<double> Mat4x2d;
typedef Mat3x4<double> Mat3x4d;
typedef Mat2x4<double> Mat2x4d;
typedef Mat4<int> Mat4i;
typedef Mat4x3<float> Mat4x3i;
typedef Mat4x2<float> Mat4x2i;
typedef Mat3x4<float> Mat3x4i;
typedef Mat2x4<float> Mat2x4i;

};

#endif //MAT_H
