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
        Vec() {}
        Vec(const Vec<T, COLS>& vec) {
            for(size_t c = 0; c < COLS; c++) {
                data[c] = vec.data[c];
            }
        }
        inline T at(const size_t col) const {
            assert(col >= 0 && col < COLS);
            return data[col];
        }
        inline void set(const size_t col, const T val) {
            assert(col >= 0 && col < COLS);
            data[col] = val;
        }
        T norm() const {
            T magnitude2;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return std::sqrt(magnitude2);
        }
        T norm2() const {
            T magnitude2;
            for(size_t c = 0; c < COLS; c++) {
                magnitude2 += data[c] * data[c];
            }
            return magnitude2;
        }
        Vec<T, COLS> normalize() const {
            T norm = norm();
            if(norm == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            return ((*this) / norm);
        }
        
        T operator[](const size_t col) const {
            assert(col >= 0 && col < COLS);
            return data[col];
        }
        T& operator[](const size_t col) {
            assert(col >= 0 && col < COLS);
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
            if(val == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            for(size_t c = 0; c < COLS; c++) {
                data[c] /= val;
            }
            return (*this);
        }
        Vec<T, COLS> operator-() {
            Vec<T, COLS> newVec;
            for(size_t c = 0; c < COLS; c++) {
                newVec[c] = -data[c];
            }
            //return newVec;
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
        T dot(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) const;
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
    }
    return newVec;
}

template<typename T, size_t COLS>
T Vec<T, COLS>::dot(const Vec<T, COLS>& vec1, const Vec<T, COLS>& vec2) const {
    T dotProduct;
    for(size_t c = 0; c < COLS; c++) {
        dotProduct = vec1.data[c] * vec2.data[c];
    }
    return dotProduct;
}

// Specifically sized vectors
template<typename T>
class Vec2 : Vec<T, 2> {
    public:
        Vec2() {}
        Vec2(const Vec<T, 2>& vec) : Vec<T, 2>::Vec(vec) {}
        Vec2(T x, T y) {
            (*this)[0] = x;
            (*this)[1] = y;
        }    
        T x() { return this->at(0); }
        T y() { return this->at(1); }
};

template<typename T>
class Vec3 : public Vec<T, 3> {
    public:
        Vec3() {}
        Vec3(const Vec<T, 3>& vec) : Vec<T, 3>::Vec(vec) {}
        Vec3(T x, T y, T z) {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
        T x() { return this->at(0); }
        T y() { return this->at(1); }
        T z() { return this->at(2); }
        
        // Cross product for Vec3
        Vec3<T> cross(const Vec3<T>& vec1, const Vec3<T>& vec2) const {
            Vec3<T> crossProduct;
            crossProduct[0] = (vec1[1] * vec2[2]) - (vec2[1] * vec1[2]);
            crossProduct[1] = (vec1[0] * vec2[2]) - (vec2[0] * vec1[2]);
            crossProduct[2] = (vec1[0] * vec2[1]) - (vec2[0] * vec1[1]);
            return crossProduct;
        }
};

template<typename T>
class Vec4 : public Vec<T, 4> {
    public:
        Vec4() {}
        Vec4(const Vec<T, 4>& vec) : Vec<T, 4>::Vec(vec) {}
        Vec4(T x, T y, T z, T w) {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }    
        T x() { return this->at(0); }
        T y() { return this->at(1); }
        T z() { return this->at(2); }
        T w() { return this->at(3); }
};

// Typedefs
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;

// Factory constructors method
/*typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;

Vec3f MakeVec2f(float x, float y);
#define Vec2f(x, y) MakeVec2f(x, y)
Vec3f MakeVec3f(float x, float y, float z);
#define mVec3f(x, y, z) MakeVec3f(x, y, z)
Vec3f MakeVec4f(float x, float y, float z, float w);
#define Vec4f(x, y, z, w) MakeVec4f(x, y, z, w)

Vec3f MakeVec2d(double x, double y);
#define Vec2d(x, y) MakeVec2d(x, y)
Vec3f MakeVec3d(double x, double y, double z);
#define Vec3d(x, y, z) MakeVec3d(x, y, z)
Vec3f MakeVec4d(double x, double y, double z, double w);
#define Vec4d(x, y, z, w) MakeVec4d(x, y, z, w)

Vec3f MakeVec2i(int x, int y);
#define Vec2i(x, y) MakeVec2i(x, y)
Vec3f MakeVec3i(int x, int y, int z);
#define Vec3i(x, y, z) MakeVec3i(x, y, z)
Vec3f MakeVec4i(int x, int y, int z, int w);
#define Vec4i(x, y, z, w) MakeVec4i(x, y, z, w)*/

#endif //VEC_H