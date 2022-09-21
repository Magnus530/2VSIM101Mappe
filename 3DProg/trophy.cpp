#include "trophy.h"

Trophy::Trophy()
{}

Trophy::Trophy(QVector3D middlePoint)
{
//    construct2(middlePoint);
}

Trophy::Trophy(QVector3D middlePoint, int color)
{
    construct2(middlePoint, color);
}

//Trophy::Trophy(QVector3D middlePoint, GLint mUniform)
//    : VisualObject(middlePoint, mUniform)
//{
////    mMMatrixUniform = mUniform;
////    construct2(middlePoint);
//}


Trophy::~Trophy()
{}

void Trophy::init(GLint matrixUniform)
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

void Trophy::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());

//    draw2();
}

float Trophy::align(float x, float y)
{
    return sin(x) + cos(y);
}

bool Trophy::checkPlayerCollision(QVector3D playerPos)
{
    std::cout << "playerposition: " << playerPos.x() << ", " << playerPos.y() << ", " << playerPos.z() << "\n";
    std::cout << "Trophy coords: " << mPos.x() << ", " << mPos.y() << ", " << mPos.z() << "\n";
    if (playerPos.x() == mPos.x() && playerPos.y() == mPos.y())
    {
        collectedTrophies++;
        std::cout << "in trophy for:" << collectedTrophies << "\n";
        return true;
    }
    else
    {
        return false;
    }
}

bool Trophy::getCollection()
{
    return collected;
}

void Trophy::setCollection()
{
    collected = true;
}

QVector3D Trophy::getTrophyPos()
{
    return mPos;
}

// Used for function.
void Trophy::construct(QVector3D middlePoint)
{
    float spacing = 0.1;

    float tDiff = (align(middlePoint.x(), middlePoint.z())) - middlePoint.y();
    mPos = {middlePoint.x(), tDiff, middlePoint.y()};

    // Vertexes

    tDiff = (align(middlePoint.x() - spacing, middlePoint.z() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, tDiff, middlePoint.y() - spacing,1,0,0,0.5,1}); // SW 0

    tDiff = (align(middlePoint.x() + spacing, middlePoint.z() - spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, tDiff, middlePoint.y() - spacing,1,0,0,0,1}); // SE 1

    tDiff = (align(middlePoint.x() + spacing, middlePoint.z() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, tDiff, middlePoint.y() + spacing,1,0,0,1,0}); // NE 2

    tDiff = (align(middlePoint.x() - spacing, middlePoint.z() + spacing)) - 0;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, tDiff, middlePoint.y() + spacing,1,0,0,0,0.5}); // NW 3

    tDiff = (align(middlePoint.x(), middlePoint.z())) + 3;
    mVertices.push_back(Vertex{middlePoint.x(), tDiff, middlePoint.y(),1,0,0,0.5,1}); // Top 4

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

// Used in terrain.
void Trophy::construct2(QVector3D middlePoint, int color)
{
    mMatrix.setToIdentity();

    setPos(middlePoint);

    float spacing = 0.1;
    float mid = 0;

    // Vertexes

    if(color == 0)
    {
        mVertices.push_back(Vertex{mid - spacing, mid, mid + spacing,1,0,0,0,0}); // SW 0
        mVertices.push_back(Vertex{mid + spacing, mid, mid + spacing,1,0,0,0,0}); // SE 1
        mVertices.push_back(Vertex{mid + spacing, mid, mid - spacing,1,0,0,0,0}); // NE 2
        mVertices.push_back(Vertex{mid - spacing, mid, mid - spacing,1,0,0,0,0}); // NW 3
        mVertices.push_back(Vertex{mid, mid + spacing*30, mid,1,0,0,0,0}); // Top 4
    }
    else if (color == 1)
    {
        mVertices.push_back(Vertex{mid - spacing, mid, mid + spacing,0,0,1,0,0}); // SW 0
        mVertices.push_back(Vertex{mid + spacing, mid, mid + spacing,0,0,1,0,0}); // SE 1
        mVertices.push_back(Vertex{mid + spacing, mid, mid - spacing,0,0,1,0,0}); // NE 2
        mVertices.push_back(Vertex{mid - spacing, mid, mid - spacing,0,0,1,0,0}); // NW 3
        mVertices.push_back(Vertex{mid, mid + spacing*30, mid,0,0,1,0,0}); // Top 4
    }

    mIndices =
    {
        0, 1, 2,
        0, 2, 3,

        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

//    std::cout << "trophy: " << getPos3D().x() << ", " << getPos3D().y() << ", " << getPos3D().z() << "\n";
}
