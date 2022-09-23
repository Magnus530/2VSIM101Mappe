#include "coordread.h"

CoordRead::CoordRead()
{}

CoordRead::CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP)
{
    setPos(mP);

    mShaderNum = shaderNum;
    mTexId = id;

    readFile(fileName);
    createGrid(5);
}

void CoordRead::readFile(std::string fileName)
{
    std::ifstream in;
    in.open(fileName.c_str());

    if (in.is_open())
    {
        std::cout << "The file: " << fileName << " is being read.\n";

        std::string x, y, z;
        std::vector<float> xCoords, yCoords, zCoords;

        while(!in.eof())
        {
            in >> x;
            in >> z;
            in >> y;

            float tempX = std::stof(x) / mScale;
            float tempY = std::stof(y) / mScale;
            float tempZ = std::stof(z) / mScale;

            xCoords.push_back(tempX);
            yCoords.push_back(tempY);
            zCoords.push_back(tempZ);

//            std::cout << "tempx: " << tempX << " tempy: " << tempY << " tempz: " << tempZ << "\n";
//            std::cout << "x: " << x << " y: " << y << " z: " << z << "\n";
        }

        xMax = *max_element(xCoords.begin(), xCoords.end());
        yMax = *max_element(yCoords.begin(), yCoords.end());
        zMax = *max_element(zCoords.begin(), zCoords.end());

        xMin = *min_element(xCoords.begin(), xCoords.end());
        yMin = *min_element(yCoords.begin(), yCoords.end());
        zMin = *min_element(zCoords.begin(), zCoords.end());

//        std::cout << "xMax: " << xMax << " yMax: " << yMax << " zMax: " << zMax << "\n";
//        std::cout << "xMin: " << xMin << " yMin: " << yMin << " zMin: " << zMin << "\n";

        for (int i = 0; i < xCoords.size(); i++)
        {
            xCoords[i] -= xMin;
            yCoords[i] -= yMin;
            zCoords[i] -= zMin;

            mVertices.push_back(Vertex{xCoords[i], yCoords[i], zCoords[i], 1, 1, 1, 0, 0});
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

void CoordRead::createGrid(float step)
{
    int xLength = 0;
    int zWidth = 0;

    for (int i = zMin; i < zMax; i += step)
    {
        for (int j = xMin; j < xMax; j += step)
        {
            gridPoints.push_back(glm::vec3{j, 0, i});
        }
    }

//    std::cout << "gridpoint size: " << gridPoints.size() << "\n";
//    std::cout << "point one: " << gridPoints[0].x << " " << gridPoints[0].y << " " << gridPoints[0].z << "\n";
//    std::cout << "point two: " << gridPoints[1].x << " " << gridPoints[1].y << " " << gridPoints[1].z << "\n";
//    std::cout << "point eight: " << gridPoints[7].x << " " << gridPoints[7].y << " " << gridPoints[7].z << "\n";
//    std::cout << "point nine: " << gridPoints[8].x << " " << gridPoints[8].y << " " << gridPoints[8].z << "\n";

    xLength = (int) (xMax - xMin) / step + 1;
    zWidth = (int) (zMax - zMin) / step + 1;

    if(gridPoints.size() > 0)
    {
        triangulate(gridPoints, xLength, zWidth);
    }
    else
    {
        mLog->logText("Failed to create gridpoints.\n", LogType::REALERROR);
    }

}

void CoordRead::triangulate(std::vector<glm::vec3> gridPoints, float length, float width)
{
//    std::cout << "length: " << length << " width: " << width << "\n";

    for (int i = 0; i < width - 1; i++)
    {
        for (int j = 0; j < length - 1; j++)
        {
            mapTriangle mapTri;
            mapTri.name = nameGen(mTriangles);
            mapTri.id = mTriangles.size();

            mapTri.v0 = gridPoints[j + i * length];
            mapTri.v1 = gridPoints[1 + j + i * length];
            mapTri.v2 = gridPoints[j + i * length + length];

            mapTri.n0 = mapTri.id + 1;

            if (j != 0)
            {
                mapTri.n1 = mapTri.id - 1;
            }
            else
            {
                mapTri.n1 = -1;
            }

            if (i != 0)
            {
                mapTri.n2 = mapTri.id - ((length - 1) * 2) + 1;
            }

            // function to find points in tri
            mTriangles.push_back(mapTri);

            mapTriangle mapTri2;
            mapTri2.name = nameGen(mTriangles);
            mapTri2.id = mTriangles.size();
            mapTri2.v0 = gridPoints[j + i * length + length];
            mapTri2.v1 = gridPoints[1 + j + i * length];
            mapTri2.v2 = gridPoints[1 + j + i * length + length];

            if (j != length - 2)
            {
                mapTri2.n0 = mapTri2.id + 1;
            }
            else
            {
                mapTri2.n0 = -1;
            }

            if (i != width - 2)
            {
                mapTri2.n1 = mapTri2.id + ((length - 1) * 2) - 1;
            }
            else
            {
                mapTri2.n1 = -1;
            }

            mapTri2.n2 = mapTri2.id - 1;

            mTriangles.push_back(mapTri2);
        }
    }
}

std::string CoordRead::nameGen(std::vector<mapTriangle> mTri)
{
    std::string tempSize = std::to_string(mTri.size());
    std::string tempS = "t" + tempSize;

    return tempS;
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
