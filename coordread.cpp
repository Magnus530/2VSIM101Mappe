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
//    pointInsert();
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
    glm::vec3 gridPoint = glm::vec3{0,0,0};


    for (int i = zMin; i < zMax; i += step)
    {
        for (int j = xMin; j < xMax; j += step)
        {
            gridPoint = glm::vec3{j - xMin, 0, i - zMin};
            gridPoints.push_back(gridPoint);
        }
    }

    std::cout << "gridpoint size: " << gridPoints.size() << "\n";
    std::cout << "vertices size: " << mVertices.size() << "\n";

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

            mTriangles.push_back(mapTri);
            k += 2;

            mapTriangle mapTri2;
            mapTri2.name = nameGen(mTriangles);
            mapTri2.id = l;

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
            l += 2;
        }
    }
}

void CoordRead::triangulateBot(std::vector<mapTriangle>& bot, std::vector<glm::vec3> gridPoints, float length, float width)
{
}

void CoordRead::triangulateTop(std::vector<mapTriangle>& top, std::vector<glm::vec3> gridPoints, float length, float width)
{
}

void CoordRead::pointInsert()
{
//    std::cout << "v0: " << mT.v0.x << " " << mT.v0.y << " " << mT.v0.z << " " << "\n";
//    std::cout << "v1: " << mT.v1.x << " " << mT.v1.y << " " << mT.v1.z << " " << "\n";
//    std::cout << "v2: " << mT.v2.x << " " << mT.v2.y << " " << mT.v2.z << " " << "\n";

    for (int i = 0; i < mVertices.size(); i++)
    {
        for (int j = 0; j < mTriangles.size(); j++)
        {
            if (boundaryCheck(mVertices[i].getVertexXYZ(), mTriangles[j]) == true)
            {
                mTriangles[j].inPoints.push_back(mVertices[i].getVertexXYZ());
            }
        }
    }
    std::cout << "points found in triangle " << mTriangles[3].id << ": " << mTriangles[3].inPoints.size() << "\n";
//    std::cout << "points found in triangle " << mT.id << ": " << mT.inPoints.size() << "\n";

}

bool CoordRead::boundaryCheck(glm::vec3 vertVec, mapTriangle mT)
{
    bool v0 = false;
    bool v1 = false;
    bool v2 = false;

    float vX = vertVec.x;
    float vZ = vertVec.z;

    float m0X = mT.v0.x;
    float m0Z = mT.v0.z;

    float m1X = mT.v1.x;
    float m1Z = mT.v1.z;

    float m2X = mT.v2.x;
    float m2Z = mT.v2.z;

//    std::cout << "point xz: " << vX << " " << vZ << "\n";

    if (mT.id % 2 == 0)
    {
//        std::cout << "mt id: " << mT.id << "\n";
        if (vX >= m0X && vZ >= m0Z)
        {
            v0 = true;
//            std::cout << "true called for e0 " << v0 << "\n";
        }

        if (vX <= m1X && vZ >= m1Z)
        {
            v1 = true;
//            std::cout << "true called for e1 " << v1 << "\n";
        }

        if (vX >= m2X && vZ <= m2Z)
        {
            v2 = true;
//            std::cout << "true called for e2 " << v2 << "\n";
        }

        if(v0 == true && v1 == true && v2 == true)
        {
//            std::cout << "point xz: " << vX << " " << vZ << " found in triangle: " << mT.id << "\n";
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
//        std::cout << "mt id: " << mT.id << "\n";
        if (vX >= m0X && vZ <= m0Z)
        {
            v0 = true;
//            std::cout << "true called for o0 " << v0 << "\n";
        }

        if (vX <= m1X && vZ >= m1Z)
        {
            v1 = true;
//            std::cout << "true called for o1 " << v1 << "\n";
        }

        if (vX <= m2X && vZ <= m2Z)
        {
            v2 = true;
//            std::cout << "true called for o2 " << v2 << "\n";
        }

        if(v0 == true && v1 == true && v2 == true)
        {
//            std::cout << "point xz: " << vX << " " << vZ << " found in triangle: " << mT.id << "\n";
            return true;
        }
        else
        {
            return false;
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
//    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}
