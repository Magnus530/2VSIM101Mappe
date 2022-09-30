#include "planes.h"

Planes::Planes()
{}

Planes::Planes(float height, float xMax, float zMax, float dist, float num, GLuint shaderNum, GLuint id)
{
    QVector3D tempPos{0, height, 0};
    setPos(tempPos);

    mShaderNum = shaderNum;
    mTexId = id;

    construct(height, xMax, zMax, dist, num);
}

void Planes::construct(float height, float xMax, float zMax, float dist, float num)
{
    for (int i = 0; i < num; i++)
    {
        constructPlane(height, xMax, zMax);
        height -= dist;
    }

    for (int i = 0; i < mPlanes.size(); i++)
    {
        mVertices.push_back(mPlanes[i].A);
        mVertices.push_back(mPlanes[i].B);
        mVertices.push_back(mPlanes[i].C);
        mVertices.push_back(mPlanes[i].D);

        for (int j = 0; j < mPlanes[i].mapIndices.size(); j++)
        {
            mIndices.push_back(mPlanes[i].mapIndices[j]);
        }
    }
}

void Planes::constructPlane(float height, float xMax, float zMax)
{
    mapPlane mP;

    mP.pos = glm::vec3{0, height, 0};

    mP.A = Vertex{0, height, 0, 1, 1, 1, 0, 1};
    mP.B = Vertex{0, height, zMax, 1, 1, 1, 1, 1};
    mP.C = Vertex{xMax, height, zMax, 1, 1, 1, 1, 0};
    mP.D = Vertex{xMax, height, 0, 1, 1, 1, 0, 0};

    glm::vec3 AB = mP.B.getVertexXYZ() - mP.A.getVertexXYZ();
    glm::vec3 AC = mP.C.getVertexXYZ() - mP.A.getVertexXYZ();
    mP.normal = glm::cross(AB, AC);
//    std::cout << "normal:" << mP.normal.x << " " << mP.normal.y << " " << mP.normal.z << "\n";

    mP.mapIndices =
    {
        0 + (GLuint) n, 1 + (GLuint) n, 3 + (GLuint) n,
        3 + (GLuint) n, 1 + (GLuint) n, 2 + (GLuint) n
    };

    mPlanes.push_back(mP);

    n += 4;
}

void Planes::init(GLint matrixUniform)
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

void Planes::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}
