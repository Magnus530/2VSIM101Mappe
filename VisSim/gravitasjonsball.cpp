#include "gravitasjonsball.h"
#include "logger.h"

GravitasjonsBall::GravitasjonsBall(int n, QVector3D vPos): OctahedronBall (n, vPos)
{
    setPos(vPos);

//    mPosition.translate(0,0,0.25);
    mScale.scale(0.25,0.25,0.25);
}

GravitasjonsBall::~GravitasjonsBall()
{}

void GravitasjonsBall::init(GLint matrixUniform)
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

void GravitasjonsBall::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}

void GravitasjonsBall::move(float dt)
{
    QVector3D gravityImpulse = mGravity * mMass * dt;

    float invMass = 1.f / mMass;
    mVelocity += gravityImpulse * invMass;

    QVector3D pos = getPos3D() + mVelocity * dt;
    setPos(pos);
}

void GravitasjonsBall::setSurface(VisualObject *surface) { triangle_surface = surface; }
