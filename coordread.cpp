#include "coordread.h"

CoordRead::CoordRead()
{}

CoordRead::CoordRead(std::string fileName, GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;

    readFile(fileName);
}

void CoordRead::readFile(std::string fileName)
{
    std::ifstream in;
    in.open(fileName.c_str());

    if (in.is_open())
    {
        std::cout << "The file: " << fileName << " is being read.\n";

        Vertex vertex;
        std::string line;
        std::string x, y, z;
        std::string keyString = " ";
        std::string keyString2 = "123456789.";

        in >> x;
        in >> y;
        in >> z;

//        in >> line;

//        std::string::size_type tempFind = line.find(keyString);

//        if (tempFind != std::string::npos)
//        {
//            x = line.substr(0, tempFind);
//            line.erase(0, x.length() + 1);

//            y = line.substr(0, tempFind);
//            line.erase(0, y.length() + 1);

//            z = line.substr(0, tempFind);
//            line.erase(0, z.length() + 1);

//    //        std::string::size_type tempFind2 = line.find(keyString2);

//    //        if (tempFind2 == std::string::npos)
//    //        {
//    //            z = line.substr(0, tempFind);
//    //            line.erase(0, z.length() + 1);
//    //        }

//        }
        std::cout << "x: " << x << " y: " << y << " z: " << z << "\n";
//        std::cout << "line: " << line << "\n";
        in.close();

        //       linstd::getline

        //           mVertices.reserve(vertexNum);
        //       for (int i=0; i< ; i++)
        //       {
        //            in >> vertex;
        //            mVertices.push_back(vertex);
        //       }

        //    //           tempIndices.reserve(indexNum);
        //       int currentIndex = 0;
        //       for (int i = 0; i < indexNum; i++)
        //       {
        //           in >> currentIndex;
        //           mIndices.push_back(currentIndex);
        //       }
    }
    else
    {
        std::cout << "Failed to read file.\n";
    }
}


void CoordRead::init(GLint matrixUniform)
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

    // 1st attribute buffer : Position
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)NULL);
    glEnableVertexAttribArray(0);

    // 2nd atrribute buffer : Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_st) );
    glEnableVertexAttribArray(2);

    // 4rth attribute buffer : Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_color) );
    glEnableVertexAttribArray(3);

    //Index buffer binding so that its easier to create objects without duplicate vertices.
    glGenBuffers( 1, &mIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW );
    glBindVertexArray(0);
}

void CoordRead::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

//    if(b_FlatShading == true)
//        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
//    else
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}
