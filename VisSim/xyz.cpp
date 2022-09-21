#include "xyz.h"

XYZ::XYZ()
{
    mVertices.push_back(Vertex{0,0,0,1,0,0,0,0});
    mVertices.push_back(Vertex{scale,0,0,1,0,0,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0,0,0});
    mVertices.push_back(Vertex{0,scale,0,0,1,0,0,0});
    mVertices.push_back(Vertex{0,0,0,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,scale,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0,0,0});
    mVertices.push_back(Vertex{-scale,0,0,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0,0,0});
    mVertices.push_back(Vertex{0,-scale,0,0,1,0,0,0});
}

XYZ::XYZ(GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;

    mVertices.push_back(Vertex{0,0,0,       1,0,0,0,1});
    mVertices.push_back(Vertex{scale,0,0,   1,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,       0,1,0,1,1});
    mVertices.push_back(Vertex{0,scale,0,   0,1,0,0,1});
    mVertices.push_back(Vertex{0,0,0,       0,0,1,1,1});
    mVertices.push_back(Vertex{0,0,scale,   0,0,1,1,0});
    mVertices.push_back(Vertex{0,0,0,       1,0,0,0,1});
    mVertices.push_back(Vertex{-scale,0,0,  1,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,       0,1,0,0,1});
    mVertices.push_back(Vertex{0,-scale,0,  0,1,0,1,0});
    mVertices.push_back(Vertex{0,0,0,       0,0,1,0,1});
    mVertices.push_back(Vertex{0,0,-scale,  0,0,1,1,0});
}

XYZ::~XYZ()
{}

void XYZ::init(GLint matrixUniform)
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

   //enable the matrixUniform
   // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

   glBindVertexArray(0);
}

void XYZ::draw()
{
   initializeOpenGLFunctions();
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_LINES, 0, mVertices.size());
}

