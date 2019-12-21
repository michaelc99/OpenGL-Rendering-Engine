#ifndef VEC_H
#define VEC_H

#include <exceptions/math_exception.h>
#include <cmath>
#include <cassert>
#include <iostream>

template<typename T, size_t COLS>
class Vec;
template<typename T, size_t COLS>
bool equalsTol(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2, const T tolerance);
template<typename T, size_t COLS>
bool operator==(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator!=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator<(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator>=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
template<typename T, size_t COLS>
bool operator<=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
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
        /*
         * Copy constructor.
         */
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
            T magnitude2;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return std::sqrt(magnitude2);
        }
        /*
         * Norm of vector squared (saves doing a std::sqrt() call).
         */
        T norm2() const {
            T magnitude2;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return magnitude2;
        }
        /*
         * Returns a normalized (norm of 1) version of this vector.
         */
        Vec<T, COLS> normalize() const {
            T normVal = norm();
            if(normVal == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            return ((*this) / normVal);
        }
        
        T operator[](const size_t col) const {
#ifdef _DEBUG
            assert(col >= 0 && col < COLS);
#endif
            return data[col];
        }
        T& operator[](const size_t col) {
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
        friend bool operator> <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend bool operator< <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend bool operator>= <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend bool operator<= <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend std::ostream& operator<< <T, COLS>(std::ostream& out, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator+ <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> (::operator- <T, COLS>)(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> operator* <T, COLS>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2);
        friend Vec<T, COLS> operator+ <T, COLS>(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> operator+ <T, COLS>(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> (::operator- <T, COLS>)(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> (::operator- <T, COLS>)(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator* <T, COLS>(const Vec<T, COLS>& vec, const T val);
        friend Vec<T, COLS> operator* <T, COLS>(const T val, const Vec<T, COLS>& vec);
        friend Vec<T, COLS> operator/ <T, COLS>(const Vec<T, COLS>& vec, const T val);
    private:
        T data[COLS];
};

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
bool operator>(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    for(size_t c = 0; c < COLS; c++) {
        if(!(vec1[c] > vec2[c])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t COLS>
bool operator<(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    for(size_t c = 0; c < COLS; c++) {
        if(!(vec1[c] < vec2[c])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t COLS>
bool operator>=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    for(size_t c = 0; c < COLS; c++) {
        if(!(vec1[c] > vec2[c] || vec1[c] == vec2[c])) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t COLS>
bool operator<=(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    for(size_t c = 0; c < COLS; c++) {
        if(!(vec1[c] < vec2[c] || vec1[c] == vec2[c])) {
            return false;
        }
    }
    return true;
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

template<typename T, size_t COLS>
T dot(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) {
    T dotProduct;
    for(size_t c = 0; c < COLS; c++) {
        dotProduct += vec1[c] * vec2[c];
    }
    return dotProduct;
}

// Cross product for Vec<T, 3>
template<typename T>
Vec<T, 3> cross(const Vec<T, 3>& vec1, const Vec<T, 3>& vec2) {
    Vec<T, 3> crossProduct;
    crossProduct[0] = (vec1[1] * vec2[2]) - (vec2[1] * vec1[2]);
    crossProduct[1] = (vec1[0] * vec2[2]) - (vec2[0] * vec1[2]);
    crossProduct[2] = (vec1[0] * vec2[1]) - (vec2[0] * vec1[1]);
    return crossProduct;
}

// Cross product for Vec<T, 4>
template<typename T>
Vec<T, 4> cross(const Vec<T, 4>& vec1, const Vec<T, 4>& vec2) {
    Vec<T, 4> crossProduct;
    crossProduct[0] = (vec1[1] * vec2[2]) - (vec2[1] * vec1[2]);
    crossProduct[1] = (vec1[0] * vec2[2]) - (vec2[0] * vec1[2]);
    crossProduct[2] = (vec1[0] * vec2[1]) - (vec2[0] * vec1[1]);
    crossProduct[3] = 1.0f;
    return crossProduct;
}

// Specifically sized vectors
template<typename T>
class Vec2 : public Vec<T, 2> {
    public:
        Vec2() {}
        Vec2(const T val) : Vec<T, 2>::Vec(val) {}
        Vec2(T x, T y) {
            (*this)[0] = x;
            (*this)[1] = y;
        }
        template<size_t N>
        explicit Vec2(const Vec<T, N>& vecN) {
            for(size_t c = 0; c < 2; c++) {
                if(c < N) {
                    (*this)[c] = vecN[c];
                }
                else {
                    (*this)[c] = (T)1.0;
                }
            }
        }
        T x() { return this->at(0); }
        T y() { return this->at(1); }
};

template<typename T>
class Vec3 : public Vec<T, 3> {
    public:
        Vec3() {}
        Vec3(const T val) : Vec<T, 3>::Vec(val) {}
        Vec3(T x, T y, T z) {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
        template<size_t N>
        explicit Vec3(const Vec<T, N>& vecN) {
            for(size_t c = 0; c < 3; c++) {
                if(c < N) {
                    (*this)[c] = vecN[c];
                }
                else {
                    (*this)[c] = (T)1.0;
                }
            }
        }
        T x() { return this->at(0); }
        T y() { return this->at(1); }
        T z() { return this->at(2); }
};

template<typename T>
class Vec4 : public Vec<T, 4> {
    public:
        Vec4() {}
        Vec4(const T val) : Vec<T, 4>::Vec(val) {}
        Vec4(T x, T y, T z, T w) {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }
        template<size_t N>
        explicit Vec4(const Vec<T, N>& vecN) {
            for(size_t c = 0; c < 4; c++) {
                if(c < N) {
                    (*this)[c] = vecN[c];
                }
                else {
                    (*this)[c] = (T)1.0;
                }
            }
        }
        T x() { return this->at(0); }
        T y() { return this->at(1); }
        T z() { return this->at(2); }
        T w() { return this->at(3); }
};

// Typedefs
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<size_t> Vec2i;

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<size_t> Vec3i;

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<size_t> Vec4i;

#endif //VEC_H
