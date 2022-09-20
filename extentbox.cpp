#include "extentbox.h"

ExtentBox::ExtentBox()
{}

ExtentBox::ExtentBox(QVector3D middlePoint)
{
    QVector3D extent = {1,1,1};

    mVertices.push_back(Vertex{middlePoint.x() - extent.x(), middlePoint.y() - extent.y(), middlePoint.z() - extent.z(),1,0,0,0,0}); // LSW 0
    mVertices.push_back(Vertex{middlePoint.x() + extent.x(), middlePoint.y() - extent.y(), middlePoint.z() - extent.z(),1,0,0,0,0}); // LSE 1
    mVertices.push_back(Vertex{middlePoint.x() + extent.x(), middlePoint.y() - extent.y(), middlePoint.z() + extent.z(),1,0,0,0,0}); // LNE 2
    mVertices.push_back(Vertex{middlePoint.x() - extent.x(), middlePoint.y() - extent.y(), middlePoint.z() + extent.z(),1,0,0,0,0}); // LNW 3

    mVertices.push_back(Vertex{middlePoint.x() - extent.x(), middlePoint.y() + extent.y(), middlePoint.z() - extent.z(),1,0,0,0,0}); // USW 4
    mVertices.push_back(Vertex{middlePoint.x() + extent.x(), middlePoint.y() + extent.y(), middlePoint.z() - extent.z(),1,0,0,0,0}); // USE 5
    mVertices.push_back(Vertex{middlePoint.x() + extent.x(), middlePoint.y() + extent.y(), middlePoint.z() + extent.z(),1,0,0,0,0}); // UNE 6
    mVertices.push_back(Vertex{middlePoint.x() - extent.x(), middlePoint.y() + extent.y(), middlePoint.z() + extent.z(),1,0,0,0,0}); // UNW 7

    mIndices =
    {
        // Bottom
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        // Sides
        0, 4,
        1, 5,
        2, 6,
        3, 7,

        // Top
        4, 5,
        5, 6,
        6, 7,
        7, 4

    };
}

ExtentBox::~ExtentBox()
{}

void ExtentBox::init(GLint matrixUniform)
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

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ExtentBox::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
