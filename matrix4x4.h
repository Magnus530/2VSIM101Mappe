#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "visualobject.h"
#include "vertex.h"
#include "vector3d.h"

class Matrix4x4
{
private:
    float m[4][4];

    enum Flag {
        Identity        = 0x0000, // Identity matrix
        Translation     = 0x0001, // Contains a translation
        Scale           = 0x0002, // Contains a scale
        Rotation2D      = 0x0004, // Contains a rotation about the Z axis
        Rotation        = 0x0008, // Contains an arbitrary rotation
        Perspective     = 0x0010, // Last row is different from (0, 0, 0, 1)
        General         = 0x001f  // General matrix, unknown contents
    };

    Flag flagBits;

    int innerLimit, outerLimit;

public:

    static const int a = 4, b = 4;
    float A[a][b];


    float& operator()(int i, int j)
    {
        return A[i][j];
    }

    Matrix4x4();
    Matrix4x4(const float& n11, const float& n12, const float& n13, const float& n14,
              const float& n21, const float& n22, const float& n23, const float& n24,
              const float& n31, const float& n32, const float& n33, const float& n34,
              const float& n41, const float& n42, const float& n43, const float& n44);

    Matrix4x4 operator*=(const Matrix4x4 other)
    {
        float num = 0;

        for (int i = 0; i < a; i++)
        {

            for (int j = 0; j < b; j++)
            {
                num = 0;
                for (int k = 0; k < b; k++)
                {
                    num += A[i][k] * other.A[k][j];
                }
                A[i][j] = num;
            }
        }

        return *this;

        // A[0][0] = ((A[0][0] * other.A[0][0]) + (A[0][1] * other.A[1][0]) + (A[0][2] * other.A[2][0]) + (A[0][3] * other.A[3][0]));
    }


    inline void setToIdentity();
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);
    void lookAt(const Vector3d& eye, const Vector3d& at, const Vector3d& up);
    void LU();
    void print();
    void mult ( const Matrix4x4 &M);
};


// QMatrix4x4 q.translate();

#endif // MATRIX4X4_H
