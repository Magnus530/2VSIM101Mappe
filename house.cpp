#include "house.h"

House::House()
{}

House::House(QVector3D middlePoint)
{
    construct2(middlePoint);
}

House::~House()
{}

void House::getTerrain(VisualObject *vPtr)
{
    if (vPtr->isTerrain(vPtr) == true)
    {
        tPtr = static_cast<Terrain*>(vPtr);
    }
}

void House::init(GLint matrixUniform)
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

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void House::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

float House::align(float x, float y)
{
    return sin(x) + cos(y);
}

float House::getSpacing()
{
    return spacing;
}

// Used for function.
void House::construct(QVector3D middlePoint)
{
    float oneFourth = spacing/4;
    float threeFourths = oneFourth*3;

    float difference = (align(middlePoint.x(), middlePoint.y())) - middlePoint.z();
    mPos = {middlePoint.x(), difference, middlePoint.y()};

    if(tPtr != nullptr)
    {
        float terrainDiff = tPtr->sampleTerrain(middlePoint.x(), middlePoint.z());

        std::cout << "terrain diff: " << terrainDiff << "\n";
    }

    // Bottom vertices

    difference = (align(middlePoint.x() - spacing, middlePoint.y() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, difference, middlePoint.y() - spacing,1,1,0,0,0}); // SW 0

    difference = (align(middlePoint.x() + spacing, middlePoint.y() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() - spacing,1,1,0,0,0}); // SE 1

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing,1,1,0,0,0}); // NE 2

    difference = (align(middlePoint.x() - spacing, middlePoint.y() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, difference, middlePoint.y() + spacing,1,1,0,0,0}); // NW 3

    // Top vertices

    difference = (align(middlePoint.x() - spacing, middlePoint.y() - spacing)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, difference, middlePoint.y() - spacing,0,1,1,0,0}); // Top SW 4

    difference = (align(middlePoint.x() + spacing, middlePoint.y() - spacing)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() - spacing,0,1,1,0,0}); // Top SE 5

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing,0,1,1,0,0}); // Top NE 6

    difference = (align(middlePoint.x() - spacing, middlePoint.y() + spacing)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, difference, middlePoint.y() + spacing,0,1,1,0,0}); // Top NW 7

    // Top point

    difference = (align(middlePoint.x(), middlePoint.y())) + 6;
    mVertices.push_back(Vertex{middlePoint.x(), difference,middlePoint.y(),1,0.5,1,0,0}); // Top 8

    // Door Vertices

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - threeFourths,1,1,0,0,0}); // Door SW 9

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth,1,1,0,0,0}); // Door SE 10

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth,0,1,1,0,0}); // Door NE 11

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) + 4;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - threeFourths,0,1,1,0,0}); // Door NW 12

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) + 4/2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth,0,1,1,0,0}); // Door ME 13

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) + 4/2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - threeFourths,0,1,1,0,0}); // Door MW 14

    mIndices =
    {
        // Floor

        0, 1, 2,
        0, 2, 3,

        // Walls

        0, 1, 5,
        0, 5, 4,

        // 2nd wall with door

        1, 9, 12,
        1, 12, 5,
        10, 2, 6,
        10, 6, 11,
        14, 13, 11,
        14, 11, 12,

        // Third and fourth walls

        2, 3, 7,
        2, 7, 6,

        3, 0, 4,
        3, 4, 7,

        // Roof

        4, 5, 8,

        // 2nd roof with door

        5, 12, 8,
        12, 11, 8,
        11, 6, 8,

        // third and fourth roof

        6, 7, 8,
        7, 4, 8
    };
}

// Used for terrain.
void House::construct2(QVector3D middlePoint)
{
    float mid = 0;
    float oneFourth = spacing/4;
    float threeFourths = oneFourth*3;

    setPos(middlePoint);

    // Bottom vertices

    mVertices.push_back(Vertex{mid - spacing, mid, mid + spacing,1,1,0,0,0}); // LSW 0
    mVertices.push_back(Vertex{mid + spacing, mid, mid + spacing,1,1,0,0,0}); // LSE 1
    mVertices.push_back(Vertex{mid + spacing, mid, mid - spacing,1,1,0,0,0}); // LNE 2
    mVertices.push_back(Vertex{mid - spacing, mid, mid - spacing,1,1,0,0,0}); // LNW 3

    // Top vertices

    mVertices.push_back(Vertex{mid - spacing, mid + spacing, mid + spacing,1,1,0,0,0}); // USW 4
    mVertices.push_back(Vertex{mid + spacing, mid + spacing, mid + spacing,1,1,0,0,0}); // USE 5
    mVertices.push_back(Vertex{mid + spacing, mid + spacing, mid - spacing,1,1,0,0,0}); // UNE 6
    mVertices.push_back(Vertex{mid - spacing, mid + spacing, mid - spacing,1,1,0,0,0}); // UNW 7

//    // Top point

    mVertices.push_back(Vertex{mid, mid + spacing*2, mid,1,0.5,1,0,0}); // Top 8

//    // Door Vertices

    mVertices.push_back(Vertex{mid + spacing, mid, mid + oneFourth,1,1,0,0,0}); // LSW 9
    mVertices.push_back(Vertex{mid + spacing, mid, mid - oneFourth,1,1,0,0,0}); // LSE 10
    mVertices.push_back(Vertex{mid + spacing, mid + spacing, mid - oneFourth,0,1,1,0,0}); // LNE 11
    mVertices.push_back(Vertex{mid + spacing, mid + spacing, mid + oneFourth,0,1,1,0,0}); // LNW 12

    mIndices =
    {
        // Floor

        0, 1, 2,
        0, 2, 3,

        // Walls

        0, 1, 5,
        0, 5, 4,

//        // 2nd wall with door

        1, 9, 12,
        1, 12, 5,

        10, 2, 6,
        10, 6, 11,

//        // Third and fourth walls

        2, 3, 7,
        2, 7, 6,

        3, 0, 4,
        3, 4, 7,

        // Roof

        4, 5, 8,
        5, 6, 8,
        6, 7, 8,
        7, 4, 8
    };
}
