#include "cube.h"

Cube::Cube()
{}

Cube::Cube(QVector3D mP)
{}

Cube::Cube(QVector3D mP, GLuint shaderNum, GLuint id)
{
    setPos(mP);
    mShaderNum = shaderNum;
    mTexId = id;
    construct();
}

// Oppgave 7
Cube::Cube(QVector3D mP, float frac)
{
    setPos(mP);
    construct2(frac);
}

// Oppgave 10 hvit farge
void Cube::construct()
{
    float mid = 0;

    mVertices.push_back(Vertex{mid - extent, mid - extent, mid - extent,1,1,1,0,0}); // LSW 0
    mVertices.push_back(Vertex{mid + extent, mid - extent, mid - extent,1,1,1,0,0}); // LSE 1
    mVertices.push_back(Vertex{mid + extent, mid - extent, mid + extent,1,1,1,0,0}); // LNE 2
    mVertices.push_back(Vertex{mid - extent, mid - extent, mid + extent,1,1,1,0,0}); // LNW 3

    mVertices.push_back (Vertex{mid - extent, mid + extent, mid - extent,1,1,1,0,0}); // USW 4
    mVertices.push_back(Vertex{mid + extent, mid + extent, mid - extent,1,1,1,0,0}); // USE 5
    mVertices.push_back(Vertex{mid + extent, mid + extent, mid + extent,1,1,1,0,0}); // UNE 6
    mVertices.push_back(Vertex{mid - extent, mid + extent, mid + extent,1,1,1,0,0}); // UNW 7

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
}

//  Oppgave 7 egen construct for å bestemme størrelse på bombe.
void Cube::construct2(float frac)
{
    float mid = 0;

    mVertices.push_back(Vertex{mid - extent/frac, mid - extent/frac, mid - extent/frac,1,1,1,0,0}); // LSW 0
    mVertices.push_back(Vertex{mid + extent/frac, mid - extent/frac, mid - extent/frac,1,1,1,0,0}); // LSE 1
    mVertices.push_back(Vertex{mid + extent/frac, mid - extent/frac, mid + extent/frac,1,1,1,0,0}); // LNE 2
    mVertices.push_back(Vertex{mid - extent/frac, mid - extent/frac, mid + extent/frac,1,1,1,0,0}); // LNW 3

    mVertices.push_back (Vertex{mid - extent/frac, mid + extent/frac, mid - extent/frac,1,1,1,0,0}); // USW 4
    mVertices.push_back(Vertex{mid + extent/frac, mid + extent/frac, mid - extent/frac,1,1,1,0,0}); // USE 5
    mVertices.push_back(Vertex{mid + extent/frac, mid + extent/frac, mid + extent/frac,1,1,1,0,0}); // UNE 6
    mVertices.push_back(Vertex{mid - extent/frac, mid + extent/frac, mid + extent/frac,1,1,1,0,0}); // UNW 7

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
}

void Cube::move(float dt)
{
    float bombSpeed = 0.04f;
    mMatrix.translate(0, -bombSpeed, 0);
}

void Cube::init(GLint matrixUniform)
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

void Cube::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

float Cube::getSpacing()
{
    return extent;
}

GLuint Cube::getTexId()
{
    return mTexId;
}
