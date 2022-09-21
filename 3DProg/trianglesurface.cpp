// Helpful sources: https://www.geeksforgeeks.org/program-to-calculate-double-integration/

#include "trianglesurface.h"

//Skriv ferdig filfunksjonene i TriangleSurface klassen.
//Lag et TriangelSurface objekt og bruk dette til å representere flaten fra matematikk 3-oppgaven.

TriangleSurface::TriangleSurface(): VisualObject()
{
    construct();
    mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;

    construct();
    mMatrix.setToIdentity();
}

void TriangleSurface::construct()
{
//    float xmin=0.0f, xmax=2.0f, ymin=0.0f, ymax=3.0f, h=0.25f;
//    for (auto x=xmin; x<xmax; x+=h)
//    {
//       for (auto y=ymin; y<ymax; y+=h)
//       {
//           float z = pow((4*x+y), 3);
//           mVertices.push_back(Vertex{x,y,z,x,y,z});
//           z = pow((4*(x+h)+y), 3);
//           mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//           z = pow((4*x+(y+h)), 3);
//           mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//           mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//           z = pow((4*(x+h)+y), 3);
//           mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//           z = pow((4*(x+h)+(y+h)), 3);
//           mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
//       }
//    }

    float xmin=-10.0f, xmax=10.0f, ymin=-10.0f, ymax=10.0f, h=0.5f;
    for (auto x=xmin; x<xmax; x+=h)
    {
       for (auto y=ymin; y<ymax; y+=h)
       {
           float z = sin(x) + cos(y);
           mVertices.push_back(Vertex{x,y,z,z,z,h+h,0,0});
           z = sin(x + h) + cos(y);
           mVertices.push_back(Vertex{x+h,y,z,z,z,h+h,1,0});
           z = sin(x) + cos(y + h);
           mVertices.push_back(Vertex{x,y+h,z,z,z,h+h,1,1});
           mVertices.push_back(Vertex{x,y+h,z,z,z,h+h,0,1});
           z = sin(x + h) + cos(y);
           mVertices.push_back(Vertex{x+h,y,z,z,z,h+h,1,1});
           z = sin(x + h) + cos(y + h);
           mVertices.push_back(Vertex{x+h,y+h,z,z,z,h+h,1,0});
       }
    }

//    std::cout << "The sum of the double integral is: " << doubleIntegralSimpson(h, h, xmin, xmax, ymin, ymax) << "\n";
}

float TriangleSurface::function(float x, float y)
{
    return pow((4*x+y), 3);
}

float TriangleSurface::doubleIntegralSimpson(float h, float k, float lx, float ux, float ly, float uy)
{
    int nx, ny;

    float z[50][50], ax[50], answer;

    nx = (ux - lx) / h + 1;
    ny = (uy - ly) / k + 1;

    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            z[i][j] = function(lx + i * h, ly + j * k);
        }
    }

    for (int i = 0; i < nx; i++)
    {
        ax[i] = 0;

        for (int j = 0; j < ny; j++)
        {
            if (j == 0 || j == ny - 1)
            {
                ax[i] += z[i][j];
            }
            else if (j % 2 == 0)
            {
                ax[i] += 2 * z[i][j];
            }
            else
            {
                ax[i] += 4 * z[i][j];
            }
        }
        ax[i] *= (k / 3);
    }

    answer = 0;

    for (int i = 0; i < nx; i++)
    {
        if (i == 0 || i == nx - 1)
        {
            answer += ax[i];
        }
        else if (i % 2 == 0)
        {
            answer += 2 * ax[i];
        }
        else
        {
            answer += 4 * ax[i];
        }
    }
    answer *= (h / 3);

    return answer;
}

TriangleSurface::TriangleSurface(std::string fileName) : VisualObject()
{
    construct();
    writeFile(fileName);
    readFile(fileName);

    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{}

void TriangleSurface::readFile(std::string fileName)
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

void TriangleSurface::writeFile(std::string fileName)
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

void TriangleSurface::init(GLint shader)
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

    // 3nd attribute buffer : S T
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

GLuint TriangleSurface::getTexId()
{
    return mTexId;
}
