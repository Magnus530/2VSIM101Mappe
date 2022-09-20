#include "objloader.h"


ObjLoader::ObjLoader(std::string filename)
{
    readFile(filename);
    mMatrix.setToIdentity();
}

ObjLoader::ObjLoader(QVector3D middlePoint, GLuint shaderNum, GLuint id, std::string filename)
{
    mShaderNum = shaderNum;
    mTexId = id;

    readFile(filename);
    mMatrix.setToIdentity();

    setPos(middlePoint);
}

ObjLoader::~ObjLoader()
{}

void ObjLoader::readFile(std::string filename)
{
    std::ifstream fileIn;
    fileIn.open(filename, std::ifstream::in);
    if (!fileIn)
    {
        qDebug() << "File is failing to open: " << QString::fromStdString(filename);
    }

    std::string oneLine;
    std::string oneWord;

    std::vector<QVector3D> tempVertices;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector3D> tempUVs;

    unsigned int temp_index = 0;

    while(std::getline(fileIn, oneLine))
    {
        std::stringstream sStream;
        sStream << oneLine;
        oneWord = "";
        sStream >> oneWord;

        if (oneWord == "#")
        {
            continue;
        }
        if (oneWord == "")
        {
            continue;
        }
        if (oneWord == "v")
        {
            QVector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ(std::stof(oneWord));

            tempVertices.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            QVector3D tempUV = {0, 0, 0};
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            QVector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ(std::stof(oneWord));

            tempNormals.push_back(tempNormal);

            continue;
        }
        if (oneWord == "f")
        {
            int index = 0, normal = 0, uv = 0;

            for (int i = 0; i < 3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentVector;

                while(std::getline(tempWord, segment, '/'))
                {
                    segmentVector.push_back(segment);
                }
                index = std::stoi(segmentVector[0]);
                if (segmentVector[1] != "")
                {
                    uv = std::stoi(segmentVector[1]);
                }
                else
                {
                    uv = 0;
                }
                normal = std::stoi(segmentVector[2]);

                index--;
                uv--;
                normal--;

                if (uv > -1)
                {
                    Vertex tempVert(tempVertices[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else
                {
                    Vertex tempVert(tempVertices[index], tempNormals[normal], QVector3D{0, 0, 0});
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    fileIn.close();
}

void ObjLoader::init(GLint matrixUniform)
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

    // 3nd attribute buffer : S T
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ObjLoader::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}

GLuint ObjLoader::getTexId()
{
    return mTexId;
}

// Oppgave 4 og 5
void ObjLoader::rotate2(int dt, VisualObject* vPtr, Camera *cam)
{
    if(dt == 1)
    {
        float degrees = (10) / M_PI;
        mMatrix.rotate(degrees, 0, 1, 0);
//        cam->mPMatrix.rotate(degrees, 0, -1, 0);
    }
    else
    {
        float degrees = (10) / M_PI;
        mMatrix.rotate(degrees, 0, -1, 0);
//        cam->mPMatrix.rotate(degrees, 0, 1, 0);
    }
}

void ObjLoader::move(float dt)
{
//    mRotation = mMatrix;
    float degrees = (180 * dt) / M_PI;
    QVector3D tempVec = {0,-1,0};

//    mRotation.translate(0, 0, 2);
//    mRotation.rotate(degrees, 0, 1, 0);
    mMatrix.translate(-0.1,0,0.1);
    mMatrix.rotate(degrees, 0, 1, 0);
//    mMatrix = mRotation;

    time += dt;

//    setPos(mPos);
}

void ObjLoader::move2(float x, float y, float z, float dt)
{
    mMatrix.translate(x, 0, z);
}
