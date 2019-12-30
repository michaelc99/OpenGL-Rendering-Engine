/*
 * Based on "Quaternions" by Ken Shoemake
 * https://www.cs.ucr.edu/~vbz/resources/quatut.pdf
 */
#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include "vector.h"
#include "matrix.h"

namespace Engine::Math {

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
        
        Quat(const Vec4<T>& vec) : dataVec(vec) {}
        
        Quat(const Quat<T>& quat) : dataVec(quat.dataVec) {}
        
        /*
         * 
         */
        Quat(const Vec3<T>& axisVec, const T theta) {
            T halfTheta = theta / (T)2.0;
            dataVec = createVec4(axisVec * (T)std::sin(halfTheta));
            dataVec[3] = (T)std::cos(halfTheta);
        }
        
        /*
         * Converts a rotation matrix to a unit quaternion. Assumes mat is a rotation matrix.
         */
        Quat(Mat4<T> mat) {
            T partialTrace = mat[0][0] + mat[1][1] + mat[2][2];
            // Take case where we divide by the largest number to prevent error or div by small number
            if(partialTrace > (T)0.0) { // |w| largest
                T wDiagSum = partialTrace + mat[3][3];
#ifdef _DEBUG
                assert(wDiagSum > (T)0.0);
#endif
                dataVec[3] = (T)0.5 * std::sqrt(wDiagSum);
                T inv4W = (T)0.25 / dataVec[3];
                dataVec[0] = inv4W * (mat[2][1] - mat[1][2]);
                dataVec[1] = inv4W * (mat[0][2] - mat[2][0]);
                dataVec[2] = inv4W * (mat[1][0] - mat[0][1]);
            }
            else if(mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2]) { // |x| largest
                T xDiagSum = mat[0][0] - mat[1][1] - mat[2][2] + mat[3][3];
#ifdef _DEBUG
                assert(xDiagSum > (T)0.0);
#endif
                dataVec[0] = (T)0.5 * std::sqrt(xDiagSum);
                T inv4X = (T)0.25 / dataVec[0];
                dataVec[1] = inv4X * (mat[1][0] + mat[0][1]);
                dataVec[2] = inv4X * (mat[2][0] + mat[0][2]);
                dataVec[3] = inv4X * (mat[2][1] - mat[1][2]);
            }
            else if(mat[1][1] > mat[2][2]) { // |y| largest
                T yDiagSum = -mat[0][0] + mat[1][1] - mat[2][2] + mat[3][3];
#ifdef _DEBUG
                assert(yDiagSum > (T)0.0);
#endif
                dataVec[1] = (T)0.5 * std::sqrt(yDiagSum);
                T inv4Y = (T)0.25 / dataVec[1];
                dataVec[0] = inv4Y * (mat[1][0] + mat[0][1]);
                dataVec[2] = inv4Y * (mat[2][1] + mat[1][2]);
                dataVec[3] = inv4Y * (mat[0][2] - mat[2][0]);
            }
            else { // |z| largest
                T zDiagSum = -mat[0][0] - mat[1][1] + mat[2][2] + mat[3][3];
#ifdef _DEBUG
                assert(zDiagSum > (T)0.0);
#endif
                dataVec[2] = (T)0.5 * std::sqrt(zDiagSum);
                T inv4Z = (T)0.25 / dataVec[2];
                dataVec[0] = inv4Z * (mat[2][0] + mat[0][2]);
                dataVec[1] = inv4Z * (mat[2][1] + mat[1][2]);
                dataVec[3] = inv4Z * (mat[1][0] - mat[0][1]);
            }
            T norm = mat[3][3];
            dataVec /= std::sqrt(norm);
        }
        
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
        
        /*
         * Returns this quaternion normalized (norm of 1).
         */
        inline Quat<T> normalize() const {
            return Quat<T>(dataVec) / (T)std::sqrt(norm());
        }
        
        inline Quat<T> conjugate() const {
            return Quat<T>(-dataVec[0], -dataVec[1], -dataVec[2], dataVec[3]);
        }
        
        inline Quat<T> inverse() const {
            return conjugate() / norm();
        }
        
        /*
         * 
         */
        Mat4<T> toRotationMatrix() const {
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
            Mat4<T> mat = createMat4(
                    (T)1.0 - s * (y * y + z * z), s * (x * y - w * z), s * (x * z + w * y), (T)0.0,
                    s * (x * y + w * z), 1 - s * (x * x + z * z), s * (y * z - w * x), (T)0.0,
                    s * (x * z - w * y), s * (y * z + w * x), 1 - s * (x * x + y * y), (T)0.0,
                    (T)0.0, (T)0.0, (T)0.0, (T)1.0
            );
            return mat;
        }
        
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
        
        Quat<T>& operator*=(const Quat<T>& quat) {
            (*this) = (*this) * quat;
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
        friend Quat<T> (Engine::Math::operator- <T>)(const Quat<T>& quat1, const Quat<T>& quat2);
        friend Quat<T> operator* <T>(const Quat<T>& quat1, const Quat<T>& quat2);
        friend Quat<T> operator* <T>(const Quat<T>& quat, const Vec3<T>& vec);
        friend Quat<T> operator* <T>(const Vec3<T>& vec, const Quat<T>& quat);
        friend Quat<T> operator* <T>(const Quat<T>& quat, const T val);
        friend Quat<T> operator* <T>(const T val, const Quat<T>& quat);
        friend Quat<T> operator/ <T>(const Quat<T>& quat, const T val);
    private:
        Vec4<T> dataVec;
};

/*
 * 
 */
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

// Explicitly typed typedefs
typedef Quat<float> Quatf;
typedef Quat<double> Quatd;

};

#endif //QUATERNION_H
