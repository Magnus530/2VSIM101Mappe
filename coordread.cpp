#include "coordread.h"

CoordRead::CoordRead()
{}

CoordRead::CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP)
{
    setPos(mP);

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

        std::string x, y, z;
        std::vector<float> xCoords;
        std::vector<float> yCoords;
        std::vector<float> zCoords;

        while(!in.eof())
        {
            in >> x;
            in >> y;
            in >> z;

            float tempX = std::stof(x);
            float tempY = std::stof(y);
            float tempZ = std::stof(z);

            xCoords.push_back(tempX);
            yCoords.push_back(tempY);
            zCoords.push_back(tempZ);

//            std::cout << "tempx: " << tempX << " tempy: " << tempY << " tempz: " << tempZ << "\n";
//            std::cout << "x: " << x << " y: " << y << " z: " << z << "\n";
        }

        float xMin = *min_element(xCoords.begin(), xCoords.end());
        float yMin = *min_element(yCoords.begin(), yCoords.end());
        float zMin = *min_element(zCoords.begin(), zCoords.end());

        int fraction = 8;

        for (int i = 0; i < xCoords.size(); i++)
        {
            xCoords[i] -= xMin;
            yCoords[i] -= yMin;
            zCoords[i] -= zMin;

            mVertices.push_back(Vertex{xCoords[i] / fraction, zCoords[i] / fraction, yCoords[i] / fraction, 1, 1, 1, 0, 0});
//            mIndices.push_back(i);

//            std::cout << "xyz: " << mVertices[i].getVertexXYZ().x << "\n";
        }

        in.close();
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
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_POINTS, 0, mVertices.size());
//    glDrawElements(GL_POINT, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
