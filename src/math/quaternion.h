/*
 * Based on "Quaternions" by Ken Shoemake
 * https://www.cs.ucr.edu/~vbz/resources/quatut.pdf
 */
#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include "vector.h"
#include "matrix.h"

template<typename T>
class Quat;
template<typename T>
bool equalsTol(const Quat<T>& quat1, const Quat<T>& quat2, const T tolerance);
template<typename T>
bool operator==(const Quat<T>& quat1, const Quat<T>& quat2);
template<typename T>
bool operator!=(const Quat<T>& quat1, const Quat<T>& quat2);
template<typename T>
std::ostream& operator<<(std::ostream& out, const Quat<T>& quat);
template<typename T>
Quat<T> operator+(const Quat<T>& quat1, const Quat<T>& quat2);
template<typename T>
Quat<T> operator-(const Quat<T>& quat1, const Quat<T>& quat2);
template<typename T>
Quat<T> operator*(const Quat<T>& quat1, const Quat<T>& quat2);
template<typename T>
Quat<T> operator*(const Quat<T>& quat, const Vec3<T>& vec);
template<typename T>
Quat<T> operator*(const Vec3<T>& vec, const Quat<T>& quat);
template<typename T>
Quat<T> operator*(const Quat<T>& quat, const T val);
template<typename T>
Quat<T> operator*(const T val, const Quat<T>& quat);
template<typename T>
Quat<T> operator/(const Quat<T>& quat, const T val);

/*
 * Quaternion is structured as [vector, scalar]:
 *      [x, y, z, w]
 */
template<typename T>
class Quat {
    public:
        Quat() : dataVec((T)0.0) {}
        
        Quat(Vec4<T> vec) : dataVec(vec) {}
        
        Quat(Quat<T> quat) : dataVec(quat.dataVec) {}
        
        inline T at(const size_t col) const {
            return dataVec[col];
        }
        
        inline Quat<T>& set(const size_t col, const T val) {
            dataVec[col] = val;
            return (*this);
        }
        
        inline T operator[](const size_t col) const {
            return dataVec[col];
        }
        
        inline T& operator[](const size_t col) {
            return dataVec[col];
        }
        
        inline T norm() const {
            return dataVec.norm2(); // Norm of quaternion = x^2 + y^2 + z^2 + w^2
        }
        
        inline Quat<T> conjugate() {
            return Quat<T>(-dataVec[0], -dataVec[1], -dataVec[2], dataVec[3]);
        }
        
        inline Quat<T> inverse() {
            return conjugate() / norm();
        }
        
        Mat4<T> toMatrix() {
            T quatNormal = norm();
            if(quatNormal == (T)0.0) {
                throw DivideByZeroException(ERROR_INFO);
            }
            T s = (T)2.0 / quatNormal;
            if(std::isinf(s)) {
                throw DivideByZeroException(ERROR_INFO);
            }
            T x = dataVec[0];
            T y = dataVec[1];
            T z = dataVec[2];
            T w = dataVec[3];
            Mat4<T> mat(
                    1 - s * (y * y + z * z), s * (x * y - w * z), s * (x * z + w * y), (T)0.0,
                    s * (x * y + w * z), 1 - s * (x * x + z * z), s * (y * z - w * x), (T)0.0,
                    s * (x * z - w * y), s * (y * z + w * x), 1 - s * (x * x + y * y), (T)0.0,
                    (T)0.0, (T)0.0, (T)0.0, (T)1.0
            );
            return mat;
        }
        
        // From matrix
        
        
        Quat<T>& operator=(const Quat<T>& quat) {
            dataVec = quat.dataVec;
            return (*this);
        }
        
        Quat<T>& operator+=(const Quat<T>& quat) {
            dataVec += quat.dataVec;
            return (*this);
        }
        
        Quat<T>& operator-=(const Quat<T>& quat) {
            dataVec -= quat.dataVec;
            return (*this);
        }
        
        Quat<T>& operator*=(const T val) {
            dataVec *= val;
            return (*this);
        }
        
        Quat<T>& operator/=(const T val) {
            Vec4<T> newVec = dataVec / val;
            dataVec = newVec;
            return (*this);
        }
        
        Quat<T> operator-() const {
            return Quat<T>(-dataVec);
        }
        
        friend bool equalsTol <T>(const Quat<T>& quat1, const Quat<T>& quat2, const T tolerance);
        friend bool operator== <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend bool operator!= <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend std::ostream& operator<< <T>(std::ostream& out, const Quat<T>& quat);
        friend Quat<T> operator+ <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend Quat<T> operator- <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend Quat<T> operator* <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend Quat<T> operator* <T>(const Quat<T>& quat, const Vec3<T>& vec);
        friend Quat<T> operator* <T>(const Vec3<T>& vec, const Quat<T>& quat);
        friend Quat<T> operator* <T>(const Quat<T>& quat, const T val);
        friend Quat<T> operator* <T>(const T val, const Quat<T>& quat);
        friend Quat<T> operator/ <T>(const Quat<T>& quat, const T val);
    private:
        Vec4<T> dataVec;
};

template<typename T>
bool equalsTol(const Quat<T>& quat1, const Quat<T>& quat2, const T tolerance) {
    return equalsTol(quat1.dataVec, quat2.dataVec);
}

template<typename T>
bool operator==(const Quat<T>& quat1, const Quat<T>& quat2) {
    return quat1.dataVec == quat2.dataVec;
}

template<typename T>
bool operator!=(const Quat<T>& quat1, const Quat<T>& quat2) {
    return quat1.dataVec != quat2.dataVec;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Quat<T>& quat) {
    out << quat.dataVec;
    return out;
}

template<typename T>
Quat<T> operator+(const Quat<T>& quat1, const Quat<T>& quat2) {
    return Quat<T>(quat1.dataVec + quat2.dataVec);
}

template<typename T>
Quat<T> operator-(const Quat<T>& quat1, const Quat<T>& quat2) {
    return Quat<T>(quat1.dataVec - quat2.dataVec);
}

template<typename T>
Quat<T> operator*(const Quat<T>& quat1, const Quat<T>& quat2) {
    Vec4<T> newVec(
            quat1[0] * quat2[3] + quat1[1] * quat2[2] - quat1[2] * quat2[1] + quat1[3] * quat2[0],
           -quat1[0] * quat2[2] + quat1[1] * quat2[3] + quat1[2] * quat2[0] + quat1[3] * quat2[1],
            quat1[0] * quat2[1] - quat1[1] * quat2[0] + quat1[2] * quat2[3] + quat1[3] * quat2[2],
           -quat1[0] * quat2[0] - quat1[1] * quat2[1] - quat1[2] * quat2[2] + quat1[3] * quat2[3]
    );
    return Quat<T>(newVec);
}

template<typename T>
Quat<T> operator*(const Quat<T>& quat, const Vec3<T>& vec) {
    Vec4<T> newVec(
            quat[1] * vec[2] - quat[2] * vec[1] + quat[3] * vec[0],
           -quat[0] * vec[2] + quat[2] * vec[0] + quat[3] * vec[1],
            quat[0] * vec[1] - quat[1] * vec[0] + quat[3] * vec[2],
           -quat[0] * vec[0] - quat[1] * vec[1] - quat[2] * vec[2]
    );
    return Quat<T>(newVec);
}

template<typename T>
Quat<T> operator*(const Vec3<T>& vec, const Quat<T>& quat) {
    Vec4<T> newVec(
            vec[0] * quat[3] + vec[1] * quat[2] - vec[2] * quat[1],
           -vec[0] * quat[2] + vec[1] * quat[3] + vec[2] * quat[0],
            vec[0] * quat[1] - vec[1] * quat[0] + vec[2] * quat[3],
           -vec[0] * quat[0] - vec[1] * quat[1] - vec[2] * quat[2]
    );
    return Quat<T>(newVec);
}

template<typename T>
Quat<T> operator*(const Quat<T>& quat, const T val) {
    return Quat<T>(quat.dataVec * val);
}

template<typename T>
Quat<T> operator*(const T val, const Quat<T>& quat) {
    return Quat<T>(quat.dataVec * val);
}

template<typename T>
Quat<T> operator/(const Quat<T>& quat, const T val) {
    return Quat<T>(quat.dataVec / val);
}


#endif //QUATERNION_H
