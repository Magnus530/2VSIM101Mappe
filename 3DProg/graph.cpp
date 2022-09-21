#include "graph.h"

Graph::Graph()
{
    construct();
}

Graph::Graph(float x, float y, float z)
    : mX{x}, mY{y}, mZ{z}
{
    mVertices.push_back(Vertex{assignCoords(mX, mY, mZ)});
    mVertices.push_back(Vertex{assignCoords(mX, mY, mZ)});
    mVertices.push_back(Vertex{assignCoords(mX, mY, mZ)});
}

Vertex Graph::assignCoords(float& x, float& y, float& z)
{
    x += 0.2;
    y += 0.2;

    Vertex v{x, y, z, 1, 0, 0,0,0};
    return v;
}

void Graph::init(GLint matrixUniform)
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

    glBindVertexArray(0);
}

void Graph::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}

void Graph::construct()
{
    float xmin= 0.0f, xmax=5.0f, ymin= 0.0f, ymax=20.0f, h=0.1f;
    for (auto x=xmin; x<xmax; x+=h)
    {
//        float y = (1.5*pow(x, 3) - (11)*pow(x, 2) + (24.5)*x - 14);
//        mVertices.push_back(Vertex{x,y,0,0,0,h+h});
//        y = (1.5*pow(x+h, 3) - (11)*pow(x+h, 2) + (24.5)*(x+h) - 14);
//        mVertices.push_back(Vertex{x+h,y,0,0,0,h+h});

//        float y = (-1.1*pow(x, 2) + 5*x - 2.5);
//        mVertices.push_back(Vertex{x,y,0,0,0,h+h});
//        y = (-1.1*pow(x+h, 2) + 5*(x+h) - 2.5);
//        mVertices.push_back(Vertex{x+h,y,0,0,0,h+h});

        float y = (1/2*pow(x, 3) - (3/2)*pow(x, 2) - (1/2)*x + 3/2);
        mVertices.push_back(Vertex{x,y,0,0,0,h+h,0,0});
        y = (1/2*pow(x+h, 3) - (3/2)*pow(x+h, 2) - (1/2)*(x+h) + 3/2);
        mVertices.push_back(Vertex{x+h,y,0,0,0,h+h,0,0});

    }
}
