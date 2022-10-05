#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cmath>
#include <QtMath>

class Vector3d
{
private:


public:
    float mX;
    float mY;
    float mZ;

    Vector3d operator+(const Vector3d& v3d) const
    {
        Vector3d v3d1;
        v3d1.mX = mX + v3d.mX;
        v3d1.mY = mY + v3d.mY;
        v3d1.mZ = mZ + v3d.mZ;
        return v3d1;
    }

    Vector3d operator-(const Vector3d& v3d) const
    {
        Vector3d v3d1;
        v3d1.mX = mX - v3d.mX;
        v3d1.mY = mY - v3d.mY;
        v3d1.mZ = mZ - v3d.mZ;
        return v3d1;
    }

    Vector3d operator*(const Vector3d& v3d) const
    {
        Vector3d v3d1;
        v3d1.mX = mX * v3d.mX;
        v3d1.mY = mY * v3d.mY;
        v3d1.mZ = mZ * v3d.mZ;
        return v3d1;
    }

    Vector3d operator/(const Vector3d& v3d) const
    {
        Vector3d v3d1;
        v3d1.mX = mX / v3d.mX;
        v3d1.mY = mY / v3d.mY;
        v3d1.mZ = mZ / v3d.mZ;
        return v3d1;
    }


    Vector3d();
    Vector3d(const float &x, const float &y, const float &z);
    // Dot product
    float scalarProduct(Vector3d& vecA, Vector3d& vecB);
    // Cross product
    Vector3d vectorProduct(Vector3d& vecA, Vector3d& VecB);
    float vectorMagnitude(Vector3d& vecA);
    Vector3d normalize(Vector3d& vecA);
};

#endif // VECTOR3D_H
