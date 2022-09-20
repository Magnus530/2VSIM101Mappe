#ifndef VECTOR4D_H
#define VECTOR4D_H


class Vector4d
{
public:
    float mX;
    float mY;
    float mZ;
    float mW;

    Vector4d operator+(const Vector4d& v4d) const
    {
        Vector4d v4d1;
        v4d1.mX = mX + v4d.mX;
        v4d1.mY = mY + v4d.mY;
        v4d1.mZ = mZ + v4d.mZ;
        v4d1.mW = mW + v4d.mW;
        return v4d1;
    }

    Vector4d operator-(const Vector4d& v4d) const
    {
        Vector4d v4d1;
        v4d1.mX = mX - v4d.mX;
        v4d1.mY = mY - v4d.mY;
        v4d1.mZ = mZ - v4d.mZ;
        v4d1.mW = mW - v4d.mW;
        return v4d1;
    }

    Vector4d operator*(const Vector4d& v4d) const
    {
        Vector4d v4d1;
        v4d1.mX = mX * v4d.mX;
        v4d1.mY = mY * v4d.mY;
        v4d1.mZ = mZ * v4d.mZ;
        v4d1.mW = mW * v4d.mW;
        return v4d1;
    }

    Vector4d operator/(const Vector4d& v4d) const
    {
        Vector4d v4d1;
        v4d1.mX = mX / v4d.mX;
        v4d1.mY = mY / v4d.mY;
        v4d1.mZ = mZ / v4d.mZ;
        v4d1.mW = mW / v4d.mW;
        return v4d1;
    }


    Vector4d();
    Vector4d(const float &x, const float &y, const float &z, const float &w);
    // Dot product
    float scalarProduct(Vector4d& vecA, Vector4d& vecB);
    // Cross product
    Vector4d vectorProduct(Vector4d& vecA, Vector4d& VecB);
    float vectorMagnitude(Vector4d& vecA);
    Vector4d normalize(Vector4d& vecA);

};

#endif // VECTOR4D_H
