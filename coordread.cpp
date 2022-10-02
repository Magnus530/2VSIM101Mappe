#include "coordread.h"

CoordRead::CoordRead()
{}

CoordRead::CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP)
{
    setPos(mP);

    mShaderNum = shaderNum;
    mTexId = id;
    int step = 5;

    readFile(fileName);
    createGrid(step);
//    pointInsert();
//    averageCalc();
//    writeAverage("../2VSIM101Mappe/squareAverageSteian_2_9_5.txt");
    readAverage("../2VSIM101Mappe/squareAverageSteian_2_9_5.txt");
    createMidGrid(step);
}

void CoordRead::readFile(std::string fileName)
{
    std::ifstream in;
    in.open(fileName.c_str());

    if (in.is_open())
    {
        std::cout << "The file: " << fileName << " is being read.\n";

        std::string x, y, z, vertSize;
        std::vector<float> xCoords, yCoords, zCoords;

        in >> vertSize;
        std::cout << "There are " << vertSize << " vertices in this file.\n";

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
        }

        xMax = *max_element(xCoords.begin(), xCoords.end());
        yMax = *max_element(yCoords.begin(), yCoords.end());
        zMax = *max_element(zCoords.begin(), zCoords.end());

        xMin = *min_element(xCoords.begin(), xCoords.end());
        yMin = *min_element(yCoords.begin(), yCoords.end());
        zMin = *min_element(zCoords.begin(), zCoords.end());

        for (int i = 0; i < xCoords.size(); i++)
        {
            xCoords[i] -= xMin;
            yCoords[i] -= yMin;
            zCoords[i] -= zMin;

            mVertices.push_back(Vertex{xCoords[i], yCoords[i], zCoords[i], 1, 1, 1, 0, 0});
        }
        lXMax = xMax - xMin;
        lZMax = zMax - zMin;

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
    glm::vec3 gridPoint = glm::vec3{0,0,0};

    for (int i = zMin; i < zMax; i += step)
    {
        for (int j = xMin; j < xMax; j += step)
        {
            gridPoint = glm::vec3{j - xMin, 0, i - zMin};
            gridPoints.push_back(gridPoint);
        }
    }

    xLength = (int) (xMax - xMin) / step + 1;
    zWidth = (int) (zMax - zMin) / step + 1;

    tXMax = (xMax - xMin) - (step / 2);
    tXMin = 0 + (step / 2);
    tZMax = (zMax - zMin) - (step / 2);
    tZMin = 0 + (step / 2);

    if(gridPoints.size() > 0)
    {
        createSquare(xLength, zWidth);
    }
    else
    {
        std::cout << "Failed to create gridpoints.\n";
    }
}

void CoordRead::createMidGrid(float step)
{
    int xLength = 0;
    int zWidth = 0;
    std::vector<glm::vec3> tempGridPoints;
    std::vector<float> tempXVals;
    std::vector<float> tempYVals;
    std::vector<float> tempZVals;

    for (int i = 0; i < mSquares.size(); i++)
    {
        tempGridPoints.push_back(mSquares[i].midPoint);
    }

    for (int i = 0; i < tempGridPoints.size(); i++)
    {
        tempXVals.push_back(tempGridPoints[i].x);
        tempYVals.push_back(tempGridPoints[i].y);
        tempZVals.push_back(tempGridPoints[i].z);
    }

    float tempXMax = *max_element(tempXVals.begin(), tempXVals.end());
    float tempYMax = *max_element(tempYVals.begin(), tempYVals.end());
    float tempZMax = *max_element(tempZVals.begin(), tempZVals.end());

    float tempXMin = *min_element(tempXVals.begin(), tempXVals.end());
    float tempYMin = *min_element(tempYVals.begin(), tempYVals.end());
    float tempZMin = *min_element(tempZVals.begin(), tempZVals.end());

    xLength = (int) (tempXMax - tempXMin) / step + 1;
    zWidth = (int) (tempZMax - tempZMin) / step + 1;

    if(gridPoints.size() > 0)
    {
        triangulate(tempGridPoints, xLength, zWidth);
    }
    else
    {
        std::cout << "Failed to create gridpoints.\n";
    }
}

void CoordRead::createSquare(float length, float width)
{
    for (int i = 0; i < width - 1; i++)
    {
        for (int j = 0; j < length - 1; j++)
        {
            mapSquare mapSq;
            mapSq.id = mSquares.size();
            mapSq.v0 = gridPoints[j + i * length];
            mapSq.v1 = gridPoints[1 + j + i * length];
            mapSq.v2 = gridPoints[1 + j + i * length + length];
            mapSq.v3 = gridPoints[j + i * length + length];

            float x = average(mapSq.v0.x, mapSq.v2.x);
            float z = average(mapSq.v0.z, mapSq.v2.z);

            mapSq.midPoint = glm::vec3{x, 0, z};
            mSquares.push_back(mapSq);
        }
    }
}

void CoordRead::triangulate(std::vector<glm::vec3> gridPoints, int length, int width)
{
    int k = 0;
    int l = 1;

    for (int i = 0; i < width - 1; i++)
    {
        for (int j = 0; j < length - 1; j++)
        {
            mapTriangle mapTri;
            mapTri.name = nameGen(mTriangles);
            mapTri.id = k;

            mapTri.v0 = gridPoints[j + i * length];
            mVertices[j + i * length].m_xyz = mapTri.v0;
            mapTri.v1 = gridPoints[1 + j + i * length];
            mVertices[1 + j + i * length].m_xyz = mapTri.v1;
            mapTri.v2 = gridPoints[j + i * length + length];
            mVertices[j + i * length + length].m_xyz = mapTri.v2;

            mapTri.A = mVertices[j + i * length];
            mapTri.B = mVertices[1 + j + i * length];
            mapTri.C = mVertices[j + i * length + length];

            glm::vec3 a = mapTri.v2 - mapTri.v0;
            glm::vec3 b = mapTri.v1 - mapTri.v0;
            glm::vec3 n = glm::vec3{0,0,0};

            if (glm::cross(a, b) == glm::vec3{0,0,0});
            {
                n.x = 0;
                n.y = 0;
                n.z = 0;
            }

            if (glm::cross(a, b) != glm::vec3{0,0,0})
            {
                n = glm::normalize(glm::cross(a, b));
            }

            mVertices[j + i * length].m_normal = glm::vec3{0,0,0};
            mVertices[j + i * length].m_normal += n;
            mVertices[j + i * length].m_st[0] = j / (float) length + sTNumBot;
            mVertices[j + i * length].m_st[1] = i / (float) width + sTNumBot;
            mIndices.push_back(j + i * length);

            mVertices[1 + j + i * length].m_normal += n;
            mVertices[1 + j + i * length].m_st[0] = j / (float) length + sTNumBot;
            mVertices[1 + j + i * length].m_st[1] = i / (float) width + sTNumBot;
            mIndices.push_back(1 + j + i * length);

            mVertices[j + i * length + length].m_normal += n;
            mVertices[j + i * length + length].m_st[0] = j / (float) length + sTNumBot;
            mVertices[j + i * length + length].m_st[1] = i / (float) width + sTNumBot;
            mIndices.push_back(j + i * length + length);

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

            mTriangles.push_back(mapTri);
            k += 2;

            mapTriangle mapTri2;
            mapTri2.name = nameGen(mTriangles);
            mapTri2.id = l;

            a = mapTri2.v0 - mapTri2.v2;
            b = mapTri2.v1 - mapTri2.v2;
            glm::vec3 n2 = glm::vec3{0,0,0};

            if (glm::cross(b, a) == glm::vec3{0,0,0});
            {
                n2.x = 0;
                n2.y = 0;
                n2.z = 0;
            }

            if (glm::cross(b, a) != glm::vec3{0,0,0})
            {
                n2 = glm::normalize(glm::cross(b, a));
            }

            mapTri2.v0 = gridPoints[j + i * length + length];
            mVertices[j + i * length + length].m_xyz = mapTri2.v0;
            mapTri2.v1 = gridPoints[1 + j + i * length];
            mVertices[1 + j + i * length].m_xyz = mapTri2.v1;
            mapTri2.v2 = gridPoints[1 + j + i * length + length];
            mVertices[1 + j + i * length + length].m_xyz = mapTri2.v2;

            mapTri2.A = mVertices[j + i * length + length];
            mapTri2.B = mVertices[1 + j + i * length];
            mapTri2.C = mVertices[1 + j + i * length + length];

            mVertices[j + i * length + length].m_normal += n2;
            mVertices[j + i * length + length].m_st[0] = j / (float) length + sTNumTop;
            mVertices[j + i * length + length].m_st[1] = i / (float) width + sTNumTop;
            mIndices.push_back(j + i * length + length);

            mVertices[1 + j + i * length].m_normal += n2;
            mVertices[1 + j + i * length].m_st[0] = j / (float) length + sTNumTop;
            mVertices[1 + j + i * length].m_st[1] = i / (float) width + sTNumTop;
            mIndices.push_back(1 + j + i * length);

            mVertices[1 + j + i * length + length].m_normal += n2;
            mVertices[1 + j + i * length + length].m_st[0] = j / (float) length + sTNumTop;
            mVertices[1 + j + i * length + length].m_st[1] = i / (float) width + sTNumTop;
            mIndices.push_back(1 + j + i * length + length);

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
            l += 2;
        }
    }

    for (unsigned int i = 0; i < mVertices.size(); i++ )
    {
        float vLen = glm::length(mVertices[i].m_normal);
        if(vLen > 0)
        {
            mVertices[i].m_normal = glm::normalize(mVertices[i].m_normal);
        }
    }
}

void CoordRead::pointInsert()
{
    for (int i = 0; i < mVertices.size(); i++)
    {
        for (int j = 0; j < mSquares.size(); j++)
        {
            if (boundaryCheck(mVertices[i].getVertexXYZ(), mSquares[j]) == true)
            {
                mSquares[j].inPoints.push_back(mVertices[i].getVertexXYZ());
            }
        }
    }
}

bool CoordRead::boundaryCheck(glm::vec3 vertVec, mapSquare mS)
{
    bool v0 = false;
    bool v1 = false;
    bool v2 = false;
    bool v3 = false;

    float vX = vertVec.x;
    float vZ = vertVec.z;

    float m0X = mS.v0.x;
    float m0Z = mS.v0.z;

    float m1X = mS.v1.x;
    float m1Z = mS.v1.z;

    float m2X = mS.v2.x;
    float m2Z = mS.v2.z;

    float m3X = mS.v3.x;
    float m3Z = mS.v3.z;

    if (vX >= m0X && vZ >= m0Z)
    {
        v0 = true;
    }

    if (vX <= m1X && vZ >= m1Z)
    {
        v1 = true;
    }

    if (vX <= m2X && vZ <= m2Z)
    {
        v2 = true;
    }

    if (vX >= m3X && vZ <= m3Z)
    {
        v3 = true;
    }

    if(v0 == true && v1 == true && v2 == true && v3 == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float CoordRead::average(float x, float y)
{
    float sum = x + y;

    return sum / 2;
}

void CoordRead::averageCalc()
{
    for (int i = 0; i < mSquares.size(); i++)
    {
        std::vector<float> tempY;
        for (int j = 0; j < mSquares[i].inPoints.size(); j++)
        {
            tempY.push_back(mSquares[i].inPoints[j].y);
        }

        float sum = 0;
        for (int k = 0; k < tempY.size(); k++)
        {
            sum += tempY[i];
        }

        if (sum == 0)
        {
            mSquares[i].midPoint.y = 0;
        }
        else
        {
            mSquares[i].midPoint.y = sum / tempY.size();
        }
    }
}

std::string CoordRead::nameGen(std::vector<mapTriangle> mTri)
{
    std::string tempSize = std::to_string(mTri.size());
    std::string tempS = "t" + tempSize;

    return tempS;
}

void CoordRead::writeAverage(std::string fileName)
{
    std::ofstream wF;

    wF.open(fileName.c_str());

    if(wF.is_open())
    {
        for (int i = 0; i < mSquares.size(); i++)
        {
            wF << mSquares[i].midPoint.x << " ";
            wF << mSquares[i].midPoint.y << " ";
            wF << mSquares[i].midPoint.z << "\n";
        }
    }
    else
    {
        std::cout << "Failed to write to file: " << fileName << ".\n";
    }

    wF.close();
}

void CoordRead::readAverage(std::string fileName)
{
    std::ifstream in;
    in.open(fileName.c_str());

    if (in.is_open())
    {
        std::cout << "The file: " << fileName << " is being read.\n";

        std::string recID, search, sub, x, y, z;

        for (int i = 0; i < mSquares.size(); i++)
        {
            in >> x;
            in >> y;
            in >> z;

            glm::vec3 tempVec = glm::vec3{std::stof(x), std::stof(y), std::stof(z)};

            mSquares[i].midPoint = tempVec;
        }
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

//    // 3nd attribute buffer : S T
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(2);

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
    if(renderArray == true)
    {
        glDrawArrays(GL_POINTS, 0, mVertices.size());
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
    }
}

GLuint CoordRead::getTexId()
{
    return mTexId;
}
