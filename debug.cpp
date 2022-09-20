#include "debug.h"

Debug::Debug()
{}

Debug::Debug(QVector3D mP)
{
    setPos(mP);
    construct();
}

void Debug::construct()
{
    float mid = 0;
    float ex = 0.25;

    mVertices.push_back(Vertex{0,0,0,1,1,1,0,0});
    mVertices.push_back(Vertex{mid + ex,0,0,1,1,1,0,0});
    mVertices.push_back(Vertex{mid - ex,0,0,1,1,1,0,0});
    mVertices.push_back(Vertex{0,mid + ex,0,1,1,1,0,0});
    mVertices.push_back(Vertex{0,mid - ex,0,1,1,1,0,0});
    mVertices.push_back(Vertex{0,0,mid + ex,1,1,1,0,0});
    mVertices.push_back(Vertex{0,0,mid - ex,1,1,1,0,0});

    mIndices =
    {
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        0, 6
    };
}

void Debug::init(GLint matrixUniform)
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

    // 1rst attribute buffer : vertices
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

void Debug::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
