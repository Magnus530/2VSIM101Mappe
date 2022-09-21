#include "lissajous.h"

Lissajous::Lissajous()
{}

Lissajous::Lissajous(std::string fileName)
{
    construct();
    writeFile(fileName);
    readFile(fileName);

    mMatrix.setToIdentity();
}

void Lissajous::construct()
{
    float a=5.f, b=4.f, h=0.1f, delta = (M_PI/2.f), n = 500.f, x, y;
    for (float t= 0.f; t<n; t+=h)
    {
        x = sin((a*t) + delta);
        y = sin(b*t);

        mVertices.push_back(Vertex{x, y, 0, x, y, x*y,0,0});
    }
}

void Lissajous::readFile(std::string fileName)
{
    std::ifstream inn;
       inn.open(fileName.c_str());

       if (inn.is_open())
       {
           int n;
           Vertex vertex;
           inn >> n;
           mVertices.reserve(n);
           for (int i=0; i<n; i++) {
                inn >> vertex;
                mVertices.push_back(vertex);
           }
           inn.close();
       }
}

void Lissajous::writeFile(std::string fileName)
{
    std::ofstream wF;

    wF.open(fileName.c_str());

    if(wF.is_open())
    {
        wF << mVertices.size() << "\n";

        for (int i = 0; i < mVertices.size(); i++)
        {
            wF << mVertices[i] << "\n";
        }
    }
    else
    {
        std::cout << "Failed to write to file.\n";
    }
    wF.close();
}

void Lissajous::init(GLint shader)
{
    mMatrixUniform = shader;

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

void Lissajous::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}
