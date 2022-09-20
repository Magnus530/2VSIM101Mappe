#include "door.h"

Door::Door()
{

}

Door::Door(QVector3D middlePoint)
{
    construct2(middlePoint);
}

void Door::init(GLint matrixUniform)
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

void Door::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

bool Door::isDoor(VisualObject* ptr)
{
    if (ptr == this)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Door::rotate(float dt)
{
    float degrees = (20 * dt) / M_PI;

    QVector3D tempVec = {0,0,0};
    float tempX = mMatrix.column(2).x();
    if (tempX > -0.999f && isOpen == false)
    {
        tempVec = {0, -1, 0};

        if(tempX < -0.98f)
        {
//            std::cout << "true\n";
            isOpen = true;
        }
    }
    else if (tempX < 0.f && isOpen == true)
    {
        tempVec = {0, 1, 0};

        if(tempX > -0.1f)
        {
//            std::cout << "false\n";
            isOpen = false;
        }
    }
        mMatrix.rotate(degrees, tempVec);
//        std::cout << "Matrix xyz: " << mMatrix.column(2).x() << ", " << mMatrix.column(3).y() << ", " << mMatrix.column(3).z() << "\n";
//        std::cout << "tempX: " << tempX << "\n";
}

float Door::align(float x, float y)
{
    return sin(x) + cos(y);
}

// Used for function.
void Door::construct(QVector3D middlePoint)
{
    float spacing = 2;
    float oneFourth = spacing/4;
    float threeFourths = oneFourth*3;

    float difference = (align(middlePoint.x(), middlePoint.y())) - middlePoint.z();
    setPos(middlePoint);
    mPos = {middlePoint.x(), middlePoint.y(), middlePoint.z()};

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - threeFourths,1,0,0,0,0}); // Door SW 0

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth,1,0,0,0,0}); // Door SE 1

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) + 4/2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth,1,0,0,0,0}); // Door NE 2

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) + 4/2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - threeFourths,1,0,0,0,0}); // Door NW 3

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/2)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/2,1,0,0,0,0}); // Door MS 4

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/0.5)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/0.5f,1,0,0,0,0}); // Door MSE 5

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/2)) + 2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/2,1,0,0,0,0}); // Door MN 6

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/0.5)) + 2;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/0.5f,1,0,0,0,0}); // Door MNE 7

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) + 1;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - threeFourths,1,0,0,0,0}); // Door MW 8

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) + 1;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth,1,1,0,0,0}); // Door ME 9

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - threeFourths)) + 0.75;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - threeFourths,1,0,0,0,0}); // Door MSW 10

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth)) + 0.75;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth,1,1,0,0,0}); // Door MSE 11

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/2)) + 0.75;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference, middlePoint.y() + spacing - oneFourth/2,1,0,0,0,0}); // Door MSW 12

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/0.5)) + 0.75;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/0.5f,1,1,0,0,0}); // Door MSE 13

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/0.5)) + 1;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/0.5f,1,1,0,0,0}); // Door MNE 14

    difference = (align(middlePoint.x() + spacing, middlePoint.y() + spacing - oneFourth/0.5)) + 1;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() + spacing - oneFourth/0.5f,1,0,0,0,0}); // Door MNW 15

    mIndices =
    {
        0, 5, 7,
        0, 7, 3,

        5, 1, 11,
        5, 11, 13,

        14, 9, 2,
        14, 2, 7,

        13, 11, 9,
        13, 9, 14
    };
}

// Used for terrain
void Door::construct2(QVector3D middlePoint)
{
    float mid = 0;
    float spacing = 4;
    float oneFourth = spacing/4;
    float twoFourths = spacing/2;

    setPos(middlePoint);

    mVertices.push_back(Vertex{mid, mid, mid - twoFourths,1,0,0,0,0}); // LSW 0
    mVertices.push_back(Vertex{mid, mid, mid,1,0,0,0,0}); // LSE 1
    mVertices.push_back(Vertex{mid, mid + spacing, mid,1,0,0,0,0}); // UNE 2
    mVertices.push_back(Vertex{mid, mid + spacing, mid - twoFourths,1,0,0,0,0}); // UNW 3

    mIndices =
    {
        0, 1, 2,
        0, 2, 3,
    };
}

bool Door::getIsOpen()
{
    return isOpen;
}
