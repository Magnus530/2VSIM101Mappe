/* Helpful source: https://blog.demofox.org/2015/07/05/the-de-casteljeau-algorithm-for-evaluating-bezier-curves/
In this code deCasteljeau's algorithm is used to draw bezier curves. */

#include "beziercurve.h"

// Oppgave 7

BezierCurve::BezierCurve()
{
//    construct();
    construct2();
}

void BezierCurve::init(GLint matrixUniform)
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

void BezierCurve::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());

}

void BezierCurve::construct()
{
    static const int arrSize = 4;
    const float points = 10;

    float h=0.25f;
    int j = 0;

    struct SPoint
    {
        float x;
        float y;
        float z;
    };

    SPoint controlPoints[arrSize] =
    {
//       {0,3},
//       {1,2},
//       {2,3},
//       {3,2}

//        {5,1},
//        {1,2},
//        {3,4}

//        {1,2},
//        {3,4},
//        {3,1}

        {7,5,6},
        {3,5,12},
        {9,5,18},
        {17,5,20}
    };

    for (int i = 0; i < arrSize; i++)
    {
//        std::cout << "this is j: " << j << " ";
        mVertices.push_back(Vertex{controlPoints[i].x, controlPoints[i].y, controlPoints[i].z, 0, 0, 1, 0, 0});

        if(!(j <= 0 || j >= arrSize - 1))
        {
            mVertices.push_back(Vertex{controlPoints[i].x, controlPoints[i].y, controlPoints[i].z, 0, 0, 1, 0, 0});
        }
        j++;
    }

    for (float x = 0; x < points - 1; x+=h)
    {
        float t = ((float)x) / (float(points - 1));
        SPoint p;
        p.x = bezierCubic(controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, controlPoints[3].x, t);
        p.y = bezierCubic(controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, controlPoints[3].y, t);
        p.z = bezierCubic(controlPoints[0].z, controlPoints[1].z, controlPoints[2].z, controlPoints[3].z, t);
        mVertices.push_back(Vertex{p.x, p.y, p.z, 1, 0, 0, 0, 0});

//        p.x = bezierQuadratic(controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, t);
//        p.y = bezierQuadratic(controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, t);
//        mVertices.push_back(Vertex{p.x, p.y, 0, 1, 0, 0, 0, 0});


//        std::cout << "time: " << t << " point (" << p.x << ", " << p.y << ")\n";

        t = ((float)x + h) / (float(points - 1));
        p.x = bezierCubic(controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, controlPoints[3].x, t);
        p.y = bezierCubic(controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, controlPoints[3].y, t);
        p.z = bezierCubic(controlPoints[0].z, controlPoints[1].z, controlPoints[2].z, controlPoints[3].z, t);
        mVertices.push_back(Vertex{p.x, p.y, p.z, 1, 0, 0, 0, 0});

//        p.x = bezierQuadratic(controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, t);
//        p.y = bezierQuadratic(controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, t);
//        mVertices.push_back(Vertex{p.x, p.y, 0, 1, 0, 0, 0, 0});

//        std::cout << "time: " << t << " point (" << p.x << ", " << p.y << ")\n";
    }

    for (GLuint i=0; i < mVertices.size(); i++)
    {
        mIndices.push_back(i);
    }
}

// Oppgave 7 og 15
void BezierCurve::construct2()
{
    float space = -0.05;
    float xmin=5.0f, xmax=20.0f, ymin=-0.0f, ymax=0.1f, h=0.5f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        for (auto y = ymin; y<ymax; y+=h)
        {
           float z = -sin(x) + cos(y);
           mVertices.push_back(Vertex{x-space,z+5,y-1,z,z,h+h,0,0});
           z = -sin(x+h) + cos(y);
           mVertices.push_back(Vertex{x+h-space,z+5,y-1,z,z,h+h,1,0});
           z = -sin(x) + cos(y+h);
           mVertices.push_back(Vertex{x-space,z+5,y+h-1,z,z,h+h,1,1});
           mVertices.push_back(Vertex{x-space,z+5,y+h-1,z,z,h+h,0,1});
           z = -sin(x+h) + cos(y);
           mVertices.push_back(Vertex{x+h-space,z+5,y-1,z,z,h+h,1,1});
           z = -sin(x+h) + cos(y+h);
           mVertices.push_back(Vertex{x+h-space,z+5,y+h-1,z,z,h+h,1,0});
        }
    }

    for (GLuint i=0; i < mVertices.size(); i++)
    {
        mIndices.push_back(i);
    }
}

// Link at top of page for deCasteljau code.
float BezierCurve::mix(float a, float b, float t)
{
    return a * (1.0f - t) + b * t;
}

float BezierCurve::bezierQuadratic(float A, float B, float C, float t)
{
    float AB = mix(A, B, t);
    float BC = mix(B, C, t);
    return mix(AB, BC, t);
}

float BezierCurve::bezierCubic(float A, float B, float C, float D, float t)
{
    float ABC = bezierQuadratic(A, B, C, t);
    float BCD = bezierQuadratic(B, C, D, t);
    return mix(ABC, BCD, t);
}

float BezierCurve::bezierQuartic(float A, float B, float C, float D, float E, float t)
{
    float ABCD = bezierCubic(A, B, C, D, t);
    float BCDE = bezierCubic(B, C, D, E, t);
    return mix(ABCD, BCDE, t);
}

