#include "light.h"

// Oppgave 3

Light::Light()
{}

Light::Light(GLuint shaderNum, GLuint id, QVector3D middlePoint)
{
    setPos(middlePoint);

    mShaderNum = shaderNum;
    mTexId = id;

    float mid = 0;

    //Bottom vertices
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y() - spacing, middlePoint.z() - spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y() - spacing, middlePoint.z() - spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y() + spacing, middlePoint.z() - spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y() + spacing, middlePoint.z() - spacing,1,1,0,0,0});

    // Top vertices
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y() - spacing, middlePoint.z() + spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y() - spacing, middlePoint.z() + spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y() + spacing, middlePoint.z() + spacing,1,1,0,0,0});
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y() + spacing, middlePoint.z() + spacing,1,1,0,0,0});

    mIndices =
    {
        // Bottom
        0, 1, 2,
        0, 2, 3,

        // Sides
        0, 1, 5,
        0, 5, 4,

        1, 2, 6,
        1, 6, 5,

        2, 3, 7,
        2, 7, 6,

        3, 0, 4,
        3, 4, 7,

        // Top
        4, 5, 6,
        4, 6, 7
    };

    mMatrix.setToIdentity();
}

bool Light::isLight(VisualObject *mObject)
{
    return true;
}

void Light::init(GLint matrixUniform)
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

void Light::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

void Light::move(float dt)
{
    float degrees = (180 * dt) / M_PI;
    QVector3D tempVec = {0,-1,0};

    mRotation.translate(0, 0, 2);
    mRotation.rotate(degrees, 0, 1, 0);
    mMatrix = mRotation;

    time += dt;
//    mPos.setZ(time * 1);
//    mPos.setX(time * 2);
    setPos(mPos);

//    mPos.setZ(sin(time) * 1);


//    mMatrix.rotate(degrees, tempVec);

//    mVelocity = QVector3D{0.1, 0.1 , 0};

//    QVector3D ds = mVelocity*dt;

//    mPos.setZ(degrees + dt);
    // mPosition = mPosition + ds;        // hvis mPosisjon er Vector3d

//    mPosition.translate(ds.x(), ds.y(), ds.z());    // hvis mPosisjon er Matrix4x4

    // normalen kan generelt være en parameter inn
//    QVector3D normal = QVector3D{0.0f, 1.0f, 0.0f};

    // bruker kryssprodukt for å finne rotasjonsvektor
//    QVector3D rotation = QVector3D::crossProduct(normal, mVelocity);
//    rotation.normalize();

    // bruk formelen ds = r dt ==> dt = ds/r
    // for å finne ut hvor mye hjulet har rotert
    // og oppdater rotasjonsmatrisen
    // husk å starte med mRotation som identitetsmatrise


//    mMatrix = mPosition*mRotation;        // hvis mPosition og mRotation er Matrix4x4

}
