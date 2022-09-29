#include "octahedronball.h"

void OctahedronBall::lagTriangel(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    Vertex v{v1.x(), v1.y(), v1.z(), 0,0,1,0,0};
    mVertices.push_back(v);
    v = Vertex{v2.x(), v2.y(), v2.z(), 0,0,1,0,0};
    mVertices.push_back(v);
    v = Vertex{v3.x(), v3.y(), v3.z(), 0,0,1,0,0};
    mVertices.push_back(v);
}

void OctahedronBall::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
    if (n>0)
    {
           QVector3D v1 = a+b; v1.normalize();
           QVector3D v2 = a+c; v2.normalize();
           QVector3D v3 = c+b; v3.normalize();
           subDivide(a, v1, v2, n-1);
           subDivide(c, v2, v3, n-1);
           subDivide(b, v3, v1, n-1);
           subDivide(v3, v2, v1, n-1);
    }
    else
    {
           lagTriangel(a, b, c);
    }
}

void OctahedronBall::oktaederUnitBall()
{    
    QVector3D v0{0, 0, 1};
    QVector3D v1{1, 0, 0};
    QVector3D v2{0, 1, 0};
    QVector3D v3{-1, 0, 0};
    QVector3D v4{0, -1, 0};
    QVector3D v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

OctahedronBall::OctahedronBall()
{}

OctahedronBall::OctahedronBall(int n, QVector3D mP) : m_rekursjoner(n), m_indeks(0), VisualObject()
{
    setPos(mP);
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
}

OctahedronBall::~OctahedronBall()
{}

void OctahedronBall::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
       initializeOpenGLFunctions();

       //Vertex Array Object - VAO
       glGenVertexArrays( 1, &mVAO );
       glBindVertexArray( mVAO );

       //Vertex Buffer Object to hold vertices - VBO
       glGenBuffers( 1, &mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, mVBO );

       glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

       glBindBuffer(GL_ARRAY_BUFFER, mVBO);
       glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
       glEnableVertexAttribArray(0);

       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       glBindVertexArray(0);
}

void OctahedronBall::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
