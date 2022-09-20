#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    int k=0;
    // legger inn data radvis
    for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
        A[i][j] = ++k;
}

Matrix4x4::Matrix4x4(const float& n11, const float& n12, const float& n13, const float& n14,
                     const float& n21, const float& n22, const float& n23, const float& n24,
                     const float& n31, const float& n32, const float& n33, const float& n34,
                     const float& n41, const float& n42, const float& n43, const float& n44)
{
    A[0][0] = n11, A[0][1] = n12, A[0][2] = n13, A[0][3] = n14,
    A[1][0] = n21, A[1][1] = n22, A[1][2] = n23, A[1][3] = n24,
    A[2][0] = n31, A[2][1] = n32, A[2][2] = n33, A[2][3] = n34,
    A[3][0] = n41, A[3][1] = n42, A[3][2] = n43, A[3][3] = n44;
}

void Matrix4x4::setToIdentity()
{
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    flagBits = Identity;
}

void Matrix4x4::translate(float x, float y, float z)
{
    Matrix4x4 tempMatrix
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    *this *= tempMatrix;
}

void Matrix4x4::rotate(float angle, float x, float y, float z)
{
    Matrix4x4 tempMatrix
    {
        cos(angle), -sin(angle), 0, 0,
        sin(angle), cos(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };


    std::cout << tempMatrix.A[0][0] << " " << tempMatrix.A[1][1] << " \n";
    std::cout << "Angle " << angle << "\n";
//    for (int i = 0; i < a; i++)
//    {
//        for (int j = 0; j < b; j++)
//        {
//            std::cout << "tempmatrix with cos: " << tempMatrix.A[i][j] << " ";
//        }
//    }

    Matrix4x4 tempMatrix2
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    *this *= tempMatrix;
    this->print();
    *this *= tempMatrix2;
}

void Matrix4x4::lookAt(const Vector3d &eye, const Vector3d &at, const Vector3d &up)
{
    Matrix4x4 M;
    M.setToIdentity();
    Vector3d v = up;
    v = v.normalize(v);          // y-akse i kameraframe
    Vector3d n = eye - at;
    n = n.normalize(n);          // z-akse i kameraframe
    Vector3d u = u.vectorProduct(v, n);
    u = u.normalize(u);          // x-akse i kameraframe
    v = v.vectorProduct(n, u);
    v = v.normalize(v);
       M(0.0f , 0.0f) = u.mX;
       M(0,1) = u.mY;  M(0,2) = u.mZ;
       M(1,0) = v.mX;  M(1,1) = v.mY;  M(1,2) = v.mZ;
       M(2,0) = n.mX;  M(2,1) = n.mY;  M(2,2) = n.mZ;
       M.translate(-eye.mX, -eye.mY, -eye.mZ);
       mult(M);        // *this * M
}

void Matrix4x4::LU()
{
    for (int k=0; k<outerLimit-1; k++)
    {
    // pivot(k);
    // Ved radoperasjoner skal vi oppnå 0-ere under diagonalelementet
    // i alle rader nedenfor (altså kolonne k
    // Vi subtraherer et multiplum av k-te
    // rad fra radene nedenfor, nuller ut kolonner fra venstre
        for (int i=k+1; i<outerLimit; i++)
        {
    // Skal mult med denne og deretter trekke fra rad i
    // denne skal bli null og vi kan lagre faktoren her
            A[i][k] = A[i][k]/A[k][k];
            for (int j=k+1; j<innerLimit; j++)
            {
    // kolonnene til høyre for den som blir nullet ut
                A[i][j] = A[i][j] - A[i][k]*A[k][j];
            }
        }
    }
}

void Matrix4x4::print()
{
    for (int i = 0; i < a; i++)
    {
        std::cout << "|";
        for (int j = 0; j < b; j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << "\t|\n";
    }
    std::cout << "\n";
}

void Matrix4x4::mult(const Matrix4x4 &M)
{
    //    this * M;
}

//QMatrix4x4 d;
//d.rotate();
