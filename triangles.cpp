#include "triangles.h"

Triangles::Triangles()
{}

Triangles::Triangles(std::string fileName, GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;
    construct(fileName);
    scale();
    mMatrix.setToIdentity();
}

void Triangles::construct(std::string fileName)
{
    float mid = 0;

    readFile(fileName);
}

void Triangles::readFile(std::string fileName)
{
    std::vector<Vertex> tempVertices;
    std::vector<GLuint> tempIndices;

    std::ifstream in;
       in.open(fileName.c_str());

       if (in.is_open())
       {
           std::cout << "The file: " << fileName << " is being read.\n";

           int vertexNum;
           int indexNum;
           Vertex vertex;
           in >> vertexNum;
           in >> indexNum;
//           std::cout << "vertexNum: " << vertexNum << "\n";
//           std::cout << "indexNum: " << indexNum << "\n";
           tempVertices.reserve(vertexNum);
           for (int i=0; i< vertexNum; i++)
           {
                in >> vertex;
                mVertices.push_back(vertex);
//                std::cout << "vertex: " << vertex << "\n";
           }

           tempIndices.reserve(indexNum);
           int currentIndex = 0;
           for (int i = 0; i < indexNum; i++)
           {
               in >> currentIndex;
               mIndices.push_back(currentIndex);
//               std::cout << "index: " << currentIndex << "\n";
           }

//           std::cout << "mVertices: " << mVertices.size() << "\n";
           for (int i = 0; i < mIndices.size(); i += 3)
           {
               Triangle triangle;

               triangle.A = mVertices[mIndices[i]];
               triangle.B = mVertices[mIndices[i+1]];
               triangle.C = mVertices[mIndices[i+2]];

               mTriangles.push_back(triangle);
           }

           in.close();
       }
       else
       {
           std::cout << "Failed to read file.\n";
       }
}

void Triangles::writeFile(std::string fileName)
{
    std::ofstream out;

    out.open(fileName.c_str());

    if(out.is_open())
    {
        std::cout << "Writing to file: " << fileName << ".\n";

        for (int i = 0; i < mVertices.size(); i++)
        {
            out << mVertices[i] << "\n";
        }

        for (int i = 0; i < mIndices.size(); i++)
        {
            out << mIndices[i] << "\n";
        }
    }
    else
    {
        std::cout << "Failed to write to file.\n";
    }
    out.close();
}

void Triangles::scale()
{
    for (int i = 0; i < mVertices.size(); i++)
    {
        mVertices[i].m_xyz = mVertices[i].getVertexXYZ() * mSize;
//        std::cout << "Xyz: " << mVertices[i].m_xyz << "\n";
    }
}

void Triangles::init(GLint matrixUniform)
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

void Triangles::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
