#include "visualleastsquaresmethod.h"

visualLeastSquaresMethod::visualLeastSquaresMethod()
{
    construct();
}

visualLeastSquaresMethod::~visualLeastSquaresMethod()
{}

void visualLeastSquaresMethod::init(GLint matrixUniform)
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

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void visualLeastSquaresMethod::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

void visualLeastSquaresMethod::construct()
{
//    mVertices.push_back(Vertex{1,1,0,1,1,1});
//    mVertices.push_back(Vertex{2,3,0,1,1,1});
//    mVertices.push_back(Vertex{3,1,0,1,1,1});
//    mVertices.push_back(Vertex{4,4,0,1,1,1});

    mVertices.push_back(Vertex{0.5,0,0,1,1,1,0,0});
    mVertices.push_back(Vertex{1,1,0,1,1,1,0,0});
    mVertices.push_back(Vertex{2,2,0,1,1,1,0,0});
    mVertices.push_back(Vertex{2,5,0,1,1,1,0,0});
    mVertices.push_back(Vertex{3,0,0,1,1,1,0,0});
    mVertices.push_back(Vertex{3,1,0,1,1,1,0,0});
    mVertices.push_back(Vertex{3,4,0,1,1,1,0,0});
    mVertices.push_back(Vertex{3,7,0,1,1,1,0,0});

//    mVertices.push_back(Vertex{-2,-15/2,0,1,1,1});
//    mVertices.push_back(Vertex{1/2,15/16,0,1,1,1});
//    mVertices.push_back(Vertex{2,-3/2,0,1,1,1});
//    mVertices.push_back(Vertex{3,0,0,1,1,1});



    for (GLuint i=0; i < mVertices.size(); i++)
    {
        mIndices.push_back(i);
    }

}
