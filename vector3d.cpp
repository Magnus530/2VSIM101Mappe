#include "vector3d.h"

Vector3d::Vector3d()
{}

Vector3d::Vector3d(const float &x, const float &y, const float &z)
    : mX{x}, mY{y}, mZ{z}
{
    //    Vector3d* vecPointer = new Vector3d;

    //    Vector3d v3d{3, 4, 2};
    //    Vector3d v3d1{2, 4, 1};

    //    float a = vecPointer->scalarProduct(v3d, v3d1);
    //    Vector3d resultVector = vecPointer->vectorProduct(v3d, v3d1);
    //    std::cout << "Scalarproduct: " << a << "\n";
    //    std::cout << "Vectorproduct: " << "[" << resultVector.mX << ", " << resultVector.mY << ", " << resultVector.mZ << "]\n";
    //    a = vecPointer->vectorMagnitude(v3d);
    //    std::cout << "Vector Magnitude: " << a << "\n";
    //    resultVector = vecPointer->normalize(v3d);
    //    std::cout << "Normalized vector: " << "[" << resultVector.mX << ", " << resultVector.mY << ", " << resultVector.mZ << "]\n";

    //    Matrix4x4* m4x4Pointer = new Matrix4x4;
    //    m4x4Pointer->print();
    //    m4x4Pointer->translate(1, 2, 3);
    //    m4x4Pointer->print();
    //    m4x4Pointer->rotate((M_PI/2), 1, 2, 3);
    //    m4x4Pointer->print();
}

float Vector3d::scalarProduct(Vector3d &vecA, Vector3d &vecB)
{
    float tempVal = (vecA.mX * vecB.mX) + (vecA.mY * vecB.mY) + (vecA.mZ * vecB.mZ);
    return tempVal;
}

Vector3d Vector3d::vectorProduct(Vector3d &vecA, Vector3d &vecB)
{
    float i, j, k;

    i = ((vecA.mY * vecB.mZ) - (vecA.mZ * vecB.mY));
    j = -1 * ((vecA.mX * vecB.mZ) - (vecA.mZ * vecB.mX));
    k = ((vecA.mX * vecB.mY) - (vecA.mY * vecB.mX));

    Vector3d tempVector{i, j, k};

    return tempVector;
}

float Vector3d::vectorMagnitude(Vector3d &vecA)
{
    float tempVal;

    tempVal = sqrt((pow(vecA.mX, 2) + (pow(vecA.mY, 2)) + (pow(vecA.mZ, 2))));
    return tempVal;
}

Vector3d Vector3d::normalize(Vector3d &vecA)
{
    float tempMagnitude;
    float i, j, k;
    tempMagnitude = vectorMagnitude(vecA);

    i = ((vecA.mX) / tempMagnitude);
    j = ((vecA.mY) / tempMagnitude);
    k = ((vecA.mZ) / tempMagnitude);

    Vector3d tempVector{i, j, k};
    return tempVector;
}

