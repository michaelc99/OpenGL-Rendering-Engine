#ifndef MAT_H
#define MAT_H

#include <exceptions/math_exception.h>
#include <math/vec.h>
#include <cmath>
#include <cassert>
#include <iostream>

template<typename T, size_t ROWS, size_t COLS>
class Mat;
template<typename T, size_t ROWS, size_t COLS>
bool equalsTol(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2, const T tolerance);
template<typename T, size_t ROWS, size_t COLS>
bool operator==(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator!=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator<(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator>=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
template<typename T, size_t ROWS, size_t COLS>
bool operator<=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
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
        /*
         * Copy constructor.
         */
        Mat(const Mat<T, ROWS, COLS>& mat) {
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r] = mat[r];
            }
        }
        
        inline T at(const size_t row, const size_t col) const {
            assert(row >= 0 && row < ROWS);
            assert(col >= 0 && col < COLS);
            return dataVecs[row][col];
        }
        inline void set(const size_t row, const size_t col, const T val) {
            assert(row >= 0 && row < ROWS);
            assert(col >= 0 && col < COLS);
            dataVecs[row][col] = val;
        }
        Vec<T, COLS> getRow(const size_t row) const {
            assert(row >= 0 && row < ROWS);
            return dataVecs[row];
        }
        void setRow(const size_t row, const Vec<T, COLS> rowVec) {
            assert(row >= 0 && row < ROWS);
            dataVecs[row] = rowVec;
        }
        Vec<T, ROWS> getCol(const size_t col) const {
            assert(col >= 0 && col < COLS);
            Vec<T, ROWS> colVec;
            for(size_t r = 0; r < ROWS; r++) {
                colVec[r] = dataVecs[r][col];
            }
            return colVec;
        }
        void setCol(const size_t col, const Vec<T, ROWS> colVec) {
            assert(col >= 0 && col < COLS);
            for(size_t r = 0; r < ROWS; r++) {
                dataVecs[r][col] = colVec[r];
            }
        }
        
        Vec<T, COLS> operator[](const size_t row) const { // Row major so [] operator returns a row
            assert(row >= 0 && row < ROWS);
            return dataVecs[row];
        }
        Vec<T, COLS>& operator[](const size_t row) { // Row major so [] operator returns a row
            assert(row >= 0 && row < ROWS);
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
        friend bool operator> <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend bool operator< <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend bool operator>= <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend bool operator<= <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend std::ostream& operator<< <T, COLS>(std::ostream& out, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> (::operator- <T, ROWS, COLS>)(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> operator+ <T, ROWS, COLS>(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> (::operator- <T, ROWS, COLS>)(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> (::operator- <T, ROWS, COLS>)(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Mat<T, ROWS, COLS> operator* <T, ROWS, COLS>(const T val, const Mat<T, ROWS, COLS>& mat);
        friend Mat<T, ROWS, COLS> operator/ <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const T val);
        friend Vec<T, COLS> operator* <T, ROWS, COLS>(const Vec<T, COLS>& vec, const Mat<T, ROWS, COLS>& mat);
        friend Vec<T, ROWS> operator* <T, ROWS, COLS>(const Mat<T, ROWS, COLS>& mat, const Vec<T, ROWS>& vec);
    private:
        Vec<T, COLS> dataVecs[ROWS];
};

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
bool operator>(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    for(size_t r = 0; r < ROWS; r++) {
        if(!(mat1[r] > mat2[r])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t ROWS, size_t COLS>
bool operator<(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    for(size_t r = 0; r < ROWS; r++) {
        if(!(mat1[r] < mat2[r])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t ROWS, size_t COLS>
bool operator>=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    for(size_t r = 0; r < ROWS; r++) {
        if(!(mat1[r] >= mat2[r])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t ROWS, size_t COLS>
bool operator<=(const Mat<T, ROWS, COLS>& mat1, const Mat<T, ROWS, COLS>& mat2) {
    for(size_t r = 0; r < ROWS; r++) {
        if(!(mat1[r] <= mat2[r])) {
            return false;
        }
    }
    return true;
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

// Specifically sized matrices
template<typename T>
class Mat2 : public Mat<T, 2, 2> {
    public:
        Mat2() {}
        Mat2(const T diagVal) : Mat<T, 2, 2>::Mat(diagVal) {}
        Mat2(const Mat<T, 2, 2>& mat) : Mat<T, 2, 2>::Mat(mat) {}
        Mat2(T x1, T y1,
             T x2, T y2) {
            (*this)[0] = Vec2<T>(x1, y1);
            (*this)[1] = Vec2<T>(x2, y2);
        }
};

template<typename T>
class Mat3 : public Mat<T, 3, 3> {
    public:
        Mat3() {}
        Mat3(const T diagVal) : Mat<T, 3, 3>::Mat(diagVal) {}
        Mat3(const Mat<T, 3, 3>& mat) : Mat<T, 3, 3>::Mat(mat) {}
        Mat3(T x1, T y1, T z1,
             T x2, T y2, T z2,
             T x3, T y3, T z3) {
            (*this)[0] = Vec3<T>(x1, y1, z1);
            (*this)[1] = Vec3<T>(x2, y2, z2);
            (*this)[2] = Vec3<T>(x3, y3, z3);
        }
};

template<typename T>
class Mat3x2 : public Mat<T, 3, 2> {
    public:
        Mat3x2() {}
        Mat3x2(const T diagVal) : Mat<T, 3, 2>::Mat(diagVal) {}
        Mat3x2(const Mat<T, 3, 2>& mat) : Mat<T, 3, 2>::Mat(mat) {}
        Mat3x2(T x1, T y1,
               T x2, T y2,
               T x3, T y3) {
            (*this)[0] = Vec2<T>(x1, y1);
            (*this)[1] = Vec2<T>(x2, y2);
            (*this)[2] = Vec2<T>(x3, y3);
        }
};

template<typename T>
class Mat2x3 : public Mat<T, 2, 3> {
    public:
        Mat2x3() {}
        Mat2x3(const T diagVal) : Mat<T, 2, 3>::Mat(diagVal) {}
        Mat2x3(const Mat<T, 2, 3>& mat) : Mat<T, 2, 3>::Mat(mat) {}
        Mat2x3(T x1, T y1, T z1,
               T x2, T y2, T z2) {
            (*this)[0] = Vec3<T>(x1, y1, z1);
            (*this)[1] = Vec3<T>(x2, y2, z2);
        }
};

template<typename T>
class Mat4 : public Mat<T, 4, 4> {
    public:
        Mat4() {}
        Mat4(const T diagVal) : Mat<T, 4, 4>::Mat(diagVal) {}
        Mat4(const Mat<T, 4, 4>& mat) : Mat<T, 4, 4>::Mat(mat) {}
        Mat4(T x1, T y1, T z1, T w1,
             T x2, T y2, T z2, T w2,
             T x3, T y3, T z3, T w3,
             T x4, T y4, T z4, T w4) {
            (*this)[0] = Vec4<T>(x1, y1, z1, w1);
            (*this)[1] = Vec4<T>(x2, y2, z2, w2);
            (*this)[2] = Vec4<T>(x3, y3, z3, w3);
            (*this)[3] = Vec4<T>(x4, y4, z4, w4);
        }
};

template<typename T>
class Mat4x3 : public Mat<T, 4, 3> {
    public:
        Mat4x3() {}
        Mat4x3(const T diagVal) : Mat<T, 4, 3>::Mat(diagVal) {}
        Mat4x3(const Mat<T, 4, 3>& mat) : Mat<T, 4, 3>::Mat(mat) {}
        Mat4x3(T x1, T y1, T z1,
               T x2, T y2, T z2,
               T x3, T y3, T z3,
               T x4, T y4, T z4) {
            (*this)[0] = Vec3<T>(x1, y1, z1);
            (*this)[1] = Vec3<T>(x2, y2, z2);
            (*this)[2] = Vec3<T>(x3, y3, z3);
            (*this)[3] = Vec3<T>(x4, y4, z4);
        }
};

template<typename T>
class Mat4x2 : public Mat<T, 4, 2> {
    public:
        Mat4x2() {}
        Mat4x2(const T diagVal) : Mat<T, 4, 2>::Mat(diagVal) {}
        Mat4x2(const Mat<T, 4, 2>& mat) : Mat<T, 4, 2>::Mat(mat) {}
        Mat4x2(T x1, T y1,
               T x2, T y2,
               T x3, T y3,
               T x4, T y4) {
            (*this)[0] = Vec2<T>(x1, y1);
            (*this)[1] = Vec2<T>(x2, y2);
            (*this)[2] = Vec2<T>(x3, y3);
            (*this)[3] = Vec2<T>(x4, y4);
        }
};

template<typename T>
class Mat3x4 : public Mat<T, 3, 4> {
    public:
        Mat3x4() {}
        Mat3x4(const T diagVal) : Mat<T, 3, 4>::Mat(diagVal) {}
        Mat3x4(const Mat<T, 3, 4>& mat) : Mat<T, 3, 4>::Mat(mat) {}
        Mat3x4(T x1, T y1, T z1, T w1,
               T x2, T y2, T z2, T w2,
               T x3, T y3, T z3, T w3) {
            (*this)[0] = Vec4<T>(x1, y1, z1, w1);
            (*this)[1] = Vec4<T>(x2, y2, z2, w2);
            (*this)[2] = Vec4<T>(x3, y3, z3, w3);
        }
};

template<typename T>
class Mat2x4 : public Mat<T, 2, 4> {
    public:
        Mat2x4() {}
        Mat2x4(const T diagVal) : Mat<T, 2, 4>::Mat(diagVal) {}
        Mat2x4(const Mat<T, 2, 4>& mat) : Mat<T, 2, 4>::Mat(mat) {}
        Mat2x4(T x1, T y1, T z1, T w1,
               T x2, T y2, T z2, T w2) {
            (*this)[0] = Vec4<T>(x1, y1, z1, w1);
            (*this)[1] = Vec4<T>(x2, y2, z2, w2);
        }
};

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

#endif //MAT_H
