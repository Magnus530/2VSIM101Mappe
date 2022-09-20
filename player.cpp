#include "player.h"

Player::Player()
{}

Player::Player(QVector3D middlePoint)
{
    construct2(middlePoint);
}

Player::~Player()
{}

void Player::init(GLint matrixUniform)
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint),
    mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Player::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

float Player::align(float x, float y)
{
    return sin(x) + cos(y);
}

// for function
void Player::move(float dx, float dy, float dz, float dt)
{
    // Oppdatere posisjon
    mX += dx;
    mY += dy;
    mZ += dz;

    float difference = align(mX, mY) - mZ;

    mZ = align(mX, mY);

    // Oppdatere mMatrix (endringer)
    mMatrix.translate(dx, dy, dz);

    mPos = {mX, mY, mZ};
}

// for terrain
void Player::move2(float x, float y, float z, float dt)
{
    // Oppdatere posisjon
    mX += x;
    mY += y;
    mZ += z;

//    std::cout << "mX: " << mX << ", mY: " << mY << ", mZ: " << mZ << "\n";

    mMatrix.translate(x, 0, z);

    //    setPos({mX, 0, mZ});
}

void Player::construct(QVector3D middlePoint)
{
    mMatrix.setToIdentity();

    float spacing = 0.1;

    mX = middlePoint.x();
    mZ = middlePoint.z();

//    float difference = (align(middlePoint.x(), middlePoint.z())) - middlePoint.y();

    setPos(middlePoint);
//    mPos = {middlePoint.x(), middlePoint.y(), middlePoint.z()};

//    difference = (align(middlePoint.x() - spacing, middlePoint.z() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y(),middlePoint.z() - spacing,0,1,0,0,0}); // SW 0

//    difference = (align(middlePoint.x() + spacing, middlePoint.z() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y(),middlePoint.z() - spacing,0,1,0,0,0}); // SE 1

//    difference = (align(middlePoint.x() + spacing, middlePoint.z() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, middlePoint.y(), middlePoint.z() + spacing,0,1,0,0,0}); // NE 2

//    difference = (align(middlePoint.x() - spacing, middlePoint.z() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, middlePoint.y(), middlePoint.z() + spacing,0,1,0,0,0}); // NW 3

//    difference = (align(middlePoint.x(), middlePoint.z())) + 2;
    mVertices.push_back(Vertex{middlePoint.x(), middlePoint.y()+2, middlePoint.z(),1,0,0,0,0}); // Top 4

    mIndices =
    {
        0, 1, 2,
        0, 2, 3,

        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };
}

void Player::construct2(QVector3D middlePoint)
{
    mMatrix.setToIdentity();

    start = middlePoint;

    float spacing = 0.1;

    mX = middlePoint.x();
    mZ = middlePoint.z();

    setPos(middlePoint);

    float mid = 0;

    // Vertexes

    mVertices.push_back(Vertex{mid - spacing, mid, mid - spacing,0,1,0,0,0}); // SW 0
    mVertices.push_back(Vertex{mid + spacing, mid, mid - spacing,0,1,0,0,0}); // SE 1
    mVertices.push_back(Vertex{mid + spacing, mid, mid + spacing,0,1,0,0,0}); // NE 2
    mVertices.push_back(Vertex{mid - spacing, mid, mid + spacing,0,1,0,0,0}); // NW 3
    mVertices.push_back(Vertex{mid, mid + spacing*20, mid,1,0,0,0,0}); // Top 4

    mIndices =
    {
        0, 1, 2,
        0, 2, 3,

        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    //    std::cout << "player: " << getPos3D().x() << ", " << getPos3D().y() << ", " << getPos3D().z() << "\n";
}

// Oppgave 9
void Player::npcMove(float dt)
{
    float speed = 0.02;
    float limitx = start.x() + 20;
    float limity = start.y() + 20;
    float limitz = start.z() + 20;
    float x = getPos3D().x();
    float y = getPos3D().y();
    float z = getPos3D().z();

//    std::cout << "z: " << z << "\n";

    if (path == 0)
    {
        if(z < limitz)
        {
            mMatrix.translate(QVector3D{0, 0, speed});
        }
        else
        {
            path++;
        }
    }
    else if(path == 1)
    {
        if(x < limitx)
        {
            mMatrix.translate(QVector3D{speed, 0, 0});
        }
        else
        {
            path++;
        }
    }
    else if(path == 2)
    {
        if(z > start.z())
        {
            mMatrix.translate(QVector3D{0, 0, -speed});
        }
        else
        {
            path++;
        }
    }
    else if(path == 3)
    {
        if(x > start.x())
        {
            mMatrix.translate(QVector3D{-speed, 0, 0});
        }
        else
        {
            path = 0;
        }
    }
}
