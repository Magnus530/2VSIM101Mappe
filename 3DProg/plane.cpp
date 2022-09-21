#include "plane.h"

Plane::Plane()
{       
    mVertices.push_back(Vertex{0,0,0,0,0,1,0,0});
    mVertices.push_back(Vertex{1,0,0,0,1,0,1,0});
    mVertices.push_back(Vertex{1,1,0,1,0,0,1,1});
    mVertices.push_back(Vertex{0,1,0,0,0,1,0,1});

    mIndices =
    {
        0, 1, 2,
        0, 2, 3
    };
}

Plane::Plane(GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;

    mVertices.push_back(Vertex{0,0,0, 0,0,1, 0,0});
    mVertices.push_back(Vertex{10,0,0, 0,0,1, 1,0});
    mVertices.push_back(Vertex{10,10,0, 0,0,1, 1,1});
    mVertices.push_back(Vertex{0,10,0, 0,0,1, 0,1});

    mIndices =
    {
        0, 1, 2,
        0, 2, 3
    };

    mMatrix.setToIdentity();
}

void Plane::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3nd attribute buffer : S T
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Plane::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

GLuint Plane::getTexId()
{
    return mTexId;
}

