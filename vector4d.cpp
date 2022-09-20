#include "vector4d.h"

Vector4d::Vector4d()
{}

Vector4d::Vector4d(const float &x, const float &y, const float &z, const float &w)
    : mX{x}, mY{y}, mZ{z}, mW{w}
{}

float Vector4d::scalarProduct(Vector4d &vecA, Vector4d &vecB)
{
    float tempVal = (vecA.mX * vecB.mX) + (vecA.mY * vecB.mY) + (vecA.mZ * vecB.mZ) + (vecA.mW * vecB.mW);
    return tempVal;
}

Vector4d Vector4d::vectorProduct(Vector4d &vecA, Vector4d &VecB)
{
//    float i, j, k, l;

//    i = ((vecA.mY * vecB.mZ) - (vecA.mZ * vecB.mY));
//    j = -1 * ((vecA.mX * vecB.mZ) - (vecA.mZ * vecB.mX));
//    k = ((vecA.mX * vecB.mY) - (vecA.mY * vecB.mX));

//    Vector3d tempVector{i, j, k};

    return vecA;

}

float Vector4d::vectorMagnitude(Vector4d &vecA)
{
 return 0;
}

Vector4d Vector4d::normalize(Vector4d &vecA)
{
 return vecA;
}
