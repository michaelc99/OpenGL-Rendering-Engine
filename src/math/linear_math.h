#ifndef LINEAR_MATH_H
#define LINEAR_MATH_H

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

namespace Engine::Math {

const float PI_CONST = 3.14159265358979323846264338328;
const float EXP_CONST = 2.71828182845904523536028747135;

template<typename T>
inline T toRadians(const T inDegrees) {
    T convConstant = (T)(PI_CONST / 180.0);
    return convConstant * inDegrees;
}

template<typename T>
inline T fromRadians(const T inRadians) {
    T convConstant = (T)(180.0 / PI_CONST);
    return convConstant * inRadians;
}

/*
 * 
 */
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

/*
 * 
 */
template<typename T>
Mat4<T> createTranslationMat(const Vec3<T>& vecToTranslateBy) {
    Mat4<T> translationMat = (Mat4<T>(1.0f).setCol(3, createVec4<T>(vecToTranslateBy)));
    return translationMat;
}

/*
 * 
 */
template<typename T>
Mat4<T> createScaleMat(const Vec3<T>& scaleVec) {
    Mat4<T> scaleMat((T)1.0);
    scaleMat[0][0] = scaleVec[0];
    scaleMat[1][1] = scaleVec[1];
    scaleMat[2][2] = scaleVec[2];
    return scaleMat;
}

/*
 * 
 */
template<typename T>
Mat4<T> createScaleMat(const Vec3<T>& scaleVec, const Vec3<T>& scaleCenterVec) {
    Mat4<T> scaleMat(
            scaleVec[0], 0.0, 0.0, (1 - scaleVec[0]) * scaleCenterVec[0],
            (T)0.0, scaleVec[1], 0.0, (1 - scaleVec[1]) * scaleCenterVec[1],
            (T)0.0, (T)0.0, scaleVec[2], (1 - scaleVec[2]) * scaleCenterVec[2],
            (T)0.0, (T)0.0, (T)0.0, (T)1.0
    );
    return scaleMat;
}

/*
 * 
 */
template<typename T>
Mat4<T> createRotationMat(const Vec3<T>& rotationAxisUnitVec, const T theta) {
    Mat4<T> rotationMat = Quat<T>(rotationAxisUnitVec.normalize(), theta).toRotationMatrix();
    return rotationMat;
}

/*
 * 
 */
template<typename T>
Mat4<T> createRotationMat(const Vec3<T>& rotationAxisUnitVec, const T theta, const Vec3<T>& rotationCenterVec) {
    Mat4<T> translationMat = createTranslationMat(-rotationCenterVec);
    Mat4<T> rotationMat = Quat<T>(rotationAxisUnitVec, theta).toRotationMatrix();
    Mat4<T> invTranslationMat = createTranslationMat(rotationCenterVec);
    rotationMat = (Mat4<T>)(invTranslationMat * rotationMat * translationMat);
    return rotationMat;
}

/*
 * 
 */
template<typename T>
Mat4<T> creatReflectionMat(const bool reflectX, const bool reflectY, const bool reflectZ) {
    Mat4<T> reflectionMat = createScaleMat(Vec3<T>((T)reflectX, (T)reflectY, (T)reflectZ));
    return reflectionMat;
}

/*
 * 
 */
/*template<typename T>
Mat4<T> createShearMat(const Vec3<T>& shearVec) {
    
}*/

/*
 * Creates a perspective projection matrix with near plane given by the rectangle botLeft to topRight at nearZ and the
 * far plane of frustum at farZ.
 * 
 * Reference: http://www.songho.ca/opengl/gl_projectionmatrix.html
 */
template<typename T>
Mat4<T> createPerspectiveProjectionMat(Vec2<T> botLeft, Vec2<T> topRight, const T nearZ, const T farZ) {
#ifdef _DEBUG
    assert(topRight[0] - botLeft[0] != (T)0.0);
    assert(topRight[1] - botLeft[1] != (T)0.0);
    assert(farZ - nearZ != (T)0.0);
#endif
    T invWidth = (T)1.0 / (topRight[0] - botLeft[0]);
    T invHeight = (T)1.0 / (topRight[1] - botLeft[1]);
    T invPlaneDist = (T)1.0 / (farZ - nearZ);
    Mat4<T> projectionMat = createMat4<T>(
            (T)2.0 * nearZ * invWidth, (T)0.0, -(topRight[0] + botLeft[0]) * invWidth, (T)0.0,
            (T)0.0, (T)2.0 * nearZ * invHeight, -(topRight[1] + botLeft[1]) * invHeight, (T)0.0,
            (T)0.0, (T)0.0, -(nearZ + farZ) * invPlaneDist, (T)-2.0 * farZ * nearZ * invPlaneDist,
            (T)0.0, (T)0.0, (T)-1.0, (T)0.0
    );
    return projectionMat;
}

/*
 * Creates a perspective projection matrix with near plane given by the rectangle botLeft to topRight at nearZ and the
 * far plane of frustum at farZ.
 * The vertical field of view is specified with verticalfieldOfView and aspect ratio is width / height.
 */
template<typename T>
Mat4<T> createPerspectiveProjectionMat(const T verticalfieldOfView, const T aspectRatio, const T nearZ, const T farZ) {
    T halfNnearPlaneHeight = (T)0.5 * nearZ * (T)std::tan(verticalfieldOfView / (T)2.0);
    T halfNearPlaneWidth = aspectRatio * halfNnearPlaneHeight;
    Mat4<T> projectionMat = createPerspectiveProjectionMat(createVec2<T>(-halfNearPlaneWidth, -halfNnearPlaneHeight), createVec2<T>(halfNearPlaneWidth, halfNnearPlaneHeight), nearZ, farZ);
    return projectionMat;
}

/*
 * Creates an orthographic (for example isometric) projection matrix for near plane rectangle defined by botLeft, topRight
 * at nearZ with the viewing frustum extending to farZ.
 */
template<typename T>
Mat4<T> createOrthoProjectionMat(Vec2<T> botLeft, Vec2<T> topRight, const T nearZ, const T farZ) {
#ifdef _DEBUG
    assert(topRight[0] - botLeft[0] != (T)0.0);
    assert(topRight[1] - botLeft[1] != (T)0.0);
    assert(farZ - nearZ != (T)0.0);
#endif
    T invWidth = (T)1.0 / (topRight[0] - botLeft[0]);
    T invHeight = (T)1.0 / (topRight[1] - botLeft[1]);
    T invPlaneDist = (T)1.0 / (farZ - nearZ);
    Mat4<T> projectionMat = createMat4<T>(
            (T)2.0 * invWidth, (T)0.0, (T)0.0, -(topRight[0] + botLeft[0]) * invWidth,
            (T)0.0, (T)2.0 * invHeight, (T)0.0, -(topRight[1] + botLeft[1]) * invHeight,
            (T)0.0, (T)0.0, (T)-2.0 * invPlaneDist, -(farZ + nearZ) * invPlaneDist,
            (T)0.0, (T)0.0, (T)0.0, (T)1.0
    );
    return projectionMat;
}

/*
 * 
 */
template<typename T>
Vec3<T> translate(const Vec3<T>& vecToTranslate, const Vec3<T>& vecToTranslateBy) {
    Vec3<T> translatedVec = createVec3<T>(createTranslationMat(vecToTranslateBy) * createVec4<T>(vecToTranslate));
    return translatedVec;
}

/*
 * 
 */
template<typename T>
Vec3<T> scale(const Vec3<T>& vecToScale, const Vec3<T>& scaleVec, const Vec3<T>& scaleCenterVec) {
    Mat4<T> scaleMat = createScaleMat(scaleCenterVec, scaleVec);
    Vec3<T> scaledVec = Vec3<T>(scaleMat * Vec4<T>(vecToScale));
    return scaledVec;
}

/*
 * Theta is the counter clockwise angle (in radians) to rotate vecToRotate by. Rotation is performed around the axis defined by
 * rotationAxisUnitVec (from origin).
 */
template<typename T>
Vec3<T> rotateAroundAxis(const Vec3<T>& vecToRotate, const Vec3<T>& rotationAxisUnitVec, const T theta) {
    Mat4<T> rotationMat = Quat<T>(rotationAxisUnitVec, theta).toRotationMatrix();
    Vec3<T> rotatedVec = Vec3<T>(rotationMat * vecToRotate);
    return rotatedVec;
}

/*
 * Theta is the counter clockwise angle (in radians) to rotate vecToRotate by. Rotation is performed around the axis defined by
 * rotationAxisUnitVec (from origin) shifted to rotationCenterVec.
 */
template<typename T>
Vec3<T> rotateAroundAxis(const Vec3<T>& vecToRotate, const Vec3<T>& rotationAxisUnitVec, const T theta, const Vec3<T>& rotationCenterVec) {
    Vec3<T> rotatedVec = translate(vecToRotate, -rotationCenterVec);
    rotatedVec = rotateAroundAxis(rotatedVec, rotationAxisUnitVec, theta);
    rotatedVec = translate(rotatedVec, rotationCenterVec);
    return rotatedVec;
}

/*
 * Theta is the counter clockwise angle (in radians) to rotate quatToRotate by. Rotation is performed around the axis defined by
 * rotationAxisUnitVec (from origin).
 */
template<typename T>
Quat<T> rotateAroundAxis(Quat<T>& quatToRotate, const Vec3<T>& rotationAxisUnitVec, const T theta) {
    Quat<T> rotationQuat = Quat<T>(rotationAxisUnitVec, theta);
    Quat<T> rotatedQuat = rotationQuat * quatToRotate * rotationQuat.conjugate();
    return rotatedQuat;
}

};

#endif //LINEAR_MATH_H
