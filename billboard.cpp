    #include "billboard.h"

Billboard::Billboard()
{}

Billboard::Billboard(QVector3D mP)
{
    construct(mP);
}

void Billboard::construct(QVector3D mP)
{
    float mid = 0;
    float extent = 1;

    setPos(mP);

    mVertices.push_back(Vertex{mid-extent, mid-extent, mid,0,0,1,1,1}); // LW
    mVertices.push_back(Vertex{mid+extent, mid-extent, mid,0,1,0,1,1}); // LE
    mVertices.push_back(Vertex{mid+extent, mid+extent, mid,1,0,0,1,1}); // UE
    mVertices.push_back(Vertex{mid-extent, mid+extent, mid,0,0,1,1,1}); // UW

    mIndices =
    {
        0, 1, 2,
        0, 2, 3
    };
}

void Billboard::init(GLint matrixUniform)
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

void Billboard::draw()
{
    initializeOpenGLFunctions();

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

void Billboard::draw2(QVector3D pos)
{
    initializeOpenGLFunctions();

    // Oppgave 11 Prøver å få til billboard.
//    glm::vec3 dir = glm::normalize(mCamPos - getPos3D);
//    mMatrix = glm::transpose(glm::lookAt(getPos3D, getPos3D + dir, {0,1,0}));
//    mMatrix = mMatrix.transposed();
//    camMatrix.setColumn(3, QVector4D{pos.x(), pos.y(), pos.z(), 1});
//    mMatrix = camMatrix.transposed();
//    dir.normalize();
//    mMatrix = camMatrix * mMatrix;

    QMatrix4x4 camMatrix;
    camMatrix.setToIdentity();
    QVector3D dir = pos - getPos3D();
    camMatrix.lookAt(getPos3D(), getPos3D() + dir, QVector3D{0, 1, 0});
    camMatrix = camMatrix.transposed();

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, camMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

GLuint Billboard::getTexId()
{
    return mTexId;
}

void Billboard::getCamPos(QVector3D camPos)
{
    mCamPos = camPos;
}

