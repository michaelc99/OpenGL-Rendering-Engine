#ifndef VEC_H
#define VEC_H

#include <exceptions/math_exception.h>
#include <cmath>
#include <cassert>
#include <iostream>

namespace Engine::Math {

template<typename T, size_t COLS>
class Vec;
template<typename T, size_t COLS>
bool equalsTol(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2, const T tolerance);
template<typename T, size_t COLS>
bool operator==(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator!=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
std::ostream& operator<<(std::ostream& out, const Vec<T, COLS>& vec);
template<typename T, size_t COLS>
Vec<T, COLS> operator+(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
Vec<T, COLS> operator-(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
Vec<T, COLS> operator+(const Vec<T, COLS>& vec, const T val);
template<typename T, size_t COLS>
Vec<T, COLS> operator+(const T val, const Vec<T, COLS>& vec);
template<typename T, size_t COLS>
Vec<T, COLS> operator-(const Vec<T, COLS>& vec, const T val);
template<typename T, size_t COLS>
Vec<T, COLS> operator-(const T val, const Vec<T, COLS>& vec);
template<typename T, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, COLS>& vec, const T val);
template<typename T, size_t COLS>
Vec<T, COLS> operator*(const T val, const Vec<T, COLS>& vec);
template<typename T, size_t COLS>
Vec<T, COLS> operator/(const Vec<T, COLS>& vec, const T val);

/*
 * Vector
 * 
 * Layout as row vector:
 *      ( x, y, z, ...)
 * 
 */
template<typename T, size_t COLS> 
class Vec {
    public:
        Vec() {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = T();
            }
        }
        
        /*
         * Constructor to set all components to value.
         */
        Vec(const T val) : Vec() {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = val;
            }
        }
        
        Vec(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = vec.data[c];
            }
        }
        
        inline T at(const size_t col) const {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            return data[col];
        }
        
        inline Vec<T, COLS>& set(const size_t col, const T val) {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            data[col] = val;
            return (*this);
        }
        
        T norm() const {
            T magnitude2 = (T)0.0;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return std::sqrt(magnitude2);
        }
        
        /*
         * Norm of vector squared.
         */
        T norm2() const {
            T magnitude2 = (T)0.0;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return magnitude2;
        }
        
        /*
         * Returns this vector normalized (norm of 1).
         */
        Vec<T, COLS> normalize() const {
            return (*this) / norm();
        }
        
        inline T operator[](const size_t col) const {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            return data[col];
        }
        
        inline T& operator[](const size_t col) {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            return data[col];
        }
        
        Vec<T, COLS>& operator=(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = vec.data[c];
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator+=(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] += vec.data[c];
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator-=(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] -= vec.data[c];
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator*=(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] *= vec.data[c];
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator=(const T val) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = val;
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator+=(const T val) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] += val;
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator-=(const T val) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] -= val;
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator*=(const T val) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] *= val;
            }
            return (*this);
        }
        
        Vec<T, COLS>& operator/=(const T val) {
            Vec<T, COLS> newVec;
            if(val == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            for(size_t c = 0; c < COLS; c++) {
                newVec[c] = data[c] / val;
                if(std::isinf(newVec[c])) {
                    throw DivideByZeroException(ERROR_INFO);
                }
            }
            (*this) = newVec;
            return (*this);
        }
        
        Vec<T, COLS> operator-() const {
            Vec<T, COLS> newVec;
            for(size_t c = 0; c < COLS; c++) {
                newVec[c] = -data[c];
            }
            return newVec;
        }
        
        friend bool equalsTol <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2, const T tolerance);
        friend bool operator== <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend bool operator!= <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend std::ostream& operator<< <T, COLS>(std::ostream& out, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator+ <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> (Engine::Math::operator- <T, COLS>)(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> operator* <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> operator+ <T, COLS>(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> operator+ <T, COLS>(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> (Engine::Math::operator- <T, COLS>)(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> (Engine::Math::operator- <T, COLS>)(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator* <T, COLS>(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> operator* <T, COLS>(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator/ <T, COLS>(const Vec<T, COLS>& vec, const T val);
    private:
        T data[COLS];
};

/*
 * 
 */
template<typename T, size_t COLS>
bool equalsTol(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2, const T tolerance) {
    if(tolerance == (T)0.0) {
        return (vec1 == vec2);
    }
    for(size_t c = 0; c < COLS; c++) {
        if((vec1[c] < vec2[c] - tolerance) || (vec1[c] > vec2[c] + tolerance)) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t COLS>
bool operator==(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    for(size_t c = 0; c < COLS; c++) {
        if(vec1[c] != vec2[c]) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t COLS>
bool operator!=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    return !(vec1 == vec2);
}

template<typename T, size_t COLS>
std::ostream& operator<<(std::ostream& out, const Vec<T, COLS>& vec) {
    out << "[";
    for(size_t c = 0; c < COLS; c++) {
        out << vec.data[c] << ((c < COLS - 1) ? ", " : "");
    }
    return out << "]";
}

template<typename T, size_t COLS>
Vec<T, COLS> operator+(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec1.data[c] + vec2.data[c];
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator-(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec1.data[c] - vec2.data[c];
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec1.data[c] * vec2.data[c];
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator+(const Vec<T, COLS>& vec, const T val) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec.data[c] + val;
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator+(const T val, const Vec<T, COLS>& vec) {
    return vec + val;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator-(const Vec<T, COLS>& vec, const T val) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec.data[c] - val;
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator-(const T val, const Vec<T, COLS>& vec) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = val - vec.data[c];
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator*(const Vec<T, COLS>& vec, const T val) {
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec.data[c] * val;
    }
    return newVec;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator*(const T val, const Vec<T, COLS>& vec) {
    return vec * val;
}

template<typename T, size_t COLS>
Vec<T, COLS> operator/(const Vec<T, COLS>& vec, const T val) {
    if(val == (T)0.0) {
        throw DivideByZeroException(ERROR_INFO);
    }
    Vec<T, COLS> newVec;
    for(size_t c = 0; c < COLS; c++) {
        newVec.data[c] = vec.data[c] / val;
        if(std::isinf(newVec.data[c])) {
            throw DivideByZeroException(ERROR_INFO);
        }
    }
    return newVec;
}

/*
 * 
 */
template<typename T, size_t COLS>
T dot(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    T dotProduct = (T)0.0;
    for(size_t c = 0; c < COLS; c++) {
        dotProduct += vec1[c] * vec2[c];
    }
    return dotProduct;
}

/*
 * Cross product for vector of length 3.
 */
template<typename T>
Vec<T, 3> cross(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    Vec<T, 3> crossProduct;
    crossProduct[0] = (vec1[1] * vec2[2]) - (vec2[1] * vec1[2]);
    crossProduct[1] = (vec1[0] * vec2[2]) - (vec2[0] * vec1[2]);
    crossProduct[2] = (vec1[0] * vec2[1]) - (vec2[0] * vec1[1]);
    return crossProduct;
}

/*
 * Cross product for vector of length 4.
 */
template<typename T>
Vec<T, 4> cross(const Vec<T, 4>& vec1, const Vec<T, 4>& vec2) {
    Vec<T, 4> crossProduct;
    crossProduct[0] = (vec1[1] * vec2[2]) - (vec2[1] * vec1[2]);
    crossProduct[1] = (vec1[0] * vec2[2]) - (vec2[0] * vec1[2]);
    crossProduct[2] = (vec1[0] * vec2[1]) - (vec2[0] * vec1[1]);
    crossProduct[3] = 1.0f;
    return crossProduct;
}

// Explicitly sized typedefs
template<typename T>
using Vec2 = Vec<T, 2>;

template<typename T>
using Vec3 = Vec<T, 3>;

template<typename T>
using Vec4 = Vec<T, 4>;

// Factory constructors
template<typename T>
Vec2<T> createVec2(T x, T y) {
    Vec2<T> vec;
    vec[0] = x;
    vec[1] = y;
    return vec;
}

template<typename T, size_t N>
Vec2<T> createVec2(const Vec<T, N>& newVecN) {
    Vec2<T> newVec;
    for(size_t c = 0; c < 2; c++) {
        if(c < N) {
            newVec[c] = newVecN[c];
        }
        else {
            newVec[c] = (T)1.0;
        }
    }
    return newVec;
}

template<typename T>
Vec3<T> createVec3(T x, T y, T z) {
    Vec3<T> newVec;
    newVec[0] = x;
    newVec[1] = y;
    newVec[2] = z;
    return newVec;
}

template<typename T, size_t N>
Vec3<T> createVec3(const Vec<T, N>& newVecN) {
    Vec3<T> newVec;
    for(size_t c = 0; c < 3; c++) {
        if(c < N) {
            newVec[c] = newVecN[c];
        }
        else {
            newVec[c] = (T)1.0;
        }
    }
    return newVec;
}

template<typename T>
Vec4<T> createVec4(T x, T y, T z, T w) {
    Vec4<T> newVec;
    newVec[0] = x;
    newVec[1] = y;
    newVec[2] = z;
    newVec[3] = w;
    return newVec;
}

template<typename T, size_t N>
Vec4<T> createVec4(const Vec<T, N>& newVecN) {
    Vec4<T> newVec;
    for(size_t c = 0; c < 4; c++) {
        if(c < N) {
            newVec[c] = newVecN[c];
        }
        else {
            newVec[c] = (T)1.0;
        }
    }
    return newVec;
}

// Explicitly typed typedefs
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;

};

#endif //VEC_H
