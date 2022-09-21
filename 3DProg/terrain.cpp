// Most of the class is the code of Hans from the project created by us in Oblig 3.
#include "terrain.h"
#include "toolbox.h"
#include <stb_image/stb_image.h>

// Oppgave 2

Terrain::Terrain()
{}

Terrain::Terrain(GLuint shaderNum, GLuint id, std::string file)
{
    mShaderNum = shaderNum;
    mTexId = id;

    m_localBuffer = stbi_load(file.c_str(), &m_width, &m_depth, (int*)NULL, 1);
    generate();
}

Terrain::~Terrain()
{}

void Terrain::init(GLint matrixUniform)
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

void Terrain::draw()
{
    glBindVertexArray( mVAO );
//    qDebug() << "Terrain Uniform = " << mMatrixUniform;
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    if(b_FlatShading == true)
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    else
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Terrain::draw(const glm::mat4 &pMat)
{
    glBindVertexArray( mVAO );
    mModelMatrix = pMat * getTransform();
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));

    if(b_FlatShading == true)
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    else
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Terrain::generate()
{
    mVertices.clear();
    mIndices.clear();

    if(b_FlatShading)
    {
        for ( int z = 0; z < m_depth; z++ )
        {
            for( int x = 0; x < m_width; x++ )
            {
                //TODO : Regne normaler til flatshading
                createPoint(x    , z    );
                createPoint(x + 1, z    );
                createPoint(x + 1, z + 1);
                createPoint(x + 1, z + 1);
                createPoint(x    , z + 1);
                createPoint(x    , z    );
            }
        }
    }
    else
    {
        for ( int z = 0; z < m_depth; z++ )
            for( int x = 0; x < m_width; x++ )
                createPoint(x,z);

        for(int j = 0; j < m_depth-1; j++)
            for(int i = 0; i < m_width-1; i++)
            {
                int it = j * m_width + i;
                Vertex v1 = mVertices[it                ];
                Vertex v2 = mVertices[it + 1            ];
                Vertex v3 = mVertices[it + 1 + m_width  ];
                Vertex v4 = mVertices[it + m_width      ];

                glm::vec3 a = v1.m_xyz - v2.m_xyz;
                glm::vec3 b = v3.m_xyz - v2.m_xyz;
                glm::vec3 n = glm::normalize(glm::cross(a,b));
                mVertices[it].m_normal                  += n; //1
                mVertices[it + 1].m_normal              += n; //2
                mVertices[it + 1 + m_width].m_normal    += n; //3

                a = v1.m_xyz - v4.m_xyz;
                b = v3.m_xyz - v4.m_xyz;
                glm::vec3 n2 = glm::normalize(glm::cross(b,a));
                mVertices[it].m_normal                  += n2; //1
                mVertices[it + 1 + m_width].m_normal    += n2; //3
                mVertices[it + m_width].m_normal        += n2; //4

                //Pushing back the triangle indices
                mIndices.push_back(it);
                mIndices.push_back(it + 1);
                mIndices.push_back(it + 1 + m_width );

                mIndices.push_back(it + 1 + m_width );
                mIndices.push_back(it + m_width );
                mIndices.push_back(it);
            }
    }
    //Setting normals to a length of 1 if they are larger than 1
    for (unsigned int i = 0; i < mVertices.size(); i++ )
    {
        float vLen = glm::length(mVertices[i].m_normal);
        if(vLen > 0)
            mVertices[i].m_normal = glm::normalize(mVertices[i].m_normal);
    }
}

void Terrain::redraw()
{
    generate();
}

void Terrain::createPoint(int x, int z)
{
    //Finds the index with width taken into account
    int it = z*m_width + x;

    //Fetches point from the texture based on th
    float PointonTexture = m_localBuffer[it];

    // Remaps the y position from the texture, to a value between 0 and 1.
    //    And it is then multiplied with a scalar that determines the height of the terrain
    float y = remap(PointonTexture, 0, 255, 0, 1) * m_yScalar;

    //Here we find the UV coordinates to the point so that we have the ability to add textures.
    //0 on the x and y axis is the bottom left! ;)
    float uCoord = (float) x / (float)(m_width - 1);
    float vCoord = (float) z / (float)(m_depth - 1);

    //Here i remap the terrain to what ever radius we desire.
    // It takes in the min and max width and depth and remaps it to a number inside the terrain radius
    float xPos = remap( x, 0, m_width, -m_terrainRadius, m_terrainRadius);
    float zPos = remap( z, 0, m_depth, -m_terrainRadius, m_terrainRadius);


    //                          x    y   z    Nr Ng Nb   U       V         Cr          Cg             Cb
    mVertices.push_back(Vertex{xPos, y, zPos, 0, 0, 0, uCoord, vCoord, y/m_yScalar, y/m_yScalar, y/m_yScalar});
    //Pushing back the vertex to the vertices array. The normals are not set here, that happens later! ;)
    //Color is a gradient based on the height of the object. That means the lower the darker, and vice versa.
}

float Terrain::sampleTerrain(const float &x, const float &z)
{
    //                     checking x bounds                  checking z bounds
    if((x < -m_terrainRadius || x > m_terrainRadius) || (z < -m_terrainRadius || z > m_terrainRadius))
        return 0;

    float yVal = 0;

    //Finding the 0 - width and 0 to height coord such that we know which vertex to sample from!
    int zPos = floor(remap( z, -m_terrainRadius, m_terrainRadius, 0, m_depth-1 ));
    int xPos = floor(remap( x, -m_terrainRadius, m_terrainRadius, 0, m_width-1 ));

    int it = (zPos * m_width) + xPos;

    // Nå har jeg vertexen i nederst til venstre(nv).
    // Nå må jeg få de andre vertexene, altså nh, th, tv

    glm::vec3 a = mVertices[it].m_xyz;
    glm::vec3 b = mVertices[it + 1].m_xyz;
    glm::vec3 c = mVertices[it + 1 + m_width].m_xyz;
    glm::vec3 d = mVertices[it + m_width].m_xyz;

    glm::vec2 p1 {a.x,a.z};
    glm::vec2 p2 {b.x,b.z};
    glm::vec2 p3 {c.x,c.z};
    glm::vec2 p4 {d.x,d.z};
    glm::vec2 p = {x, z};

    glm::vec3 baryCen = BarycentricCoordinates(p1,p2,p3,p);
    if(validateBarycentricCoordinates(baryCen))
    {
        yVal += a.y * baryCen.x;
        yVal += b.y * baryCen.y;
        yVal += c.y * baryCen.z;
        return yVal;
    }
    else
    {
        glm::vec3 baryCen = BarycentricCoordinates(p1,p3,p4,p);
        yVal += a.y * baryCen.x;
        yVal += c.y * baryCen.y;
        yVal += d.y * baryCen.z;
        return yVal;
    }
}

void Terrain::toggleShading()
{
    b_FlatShading = !b_FlatShading;
    redraw();
}

void Terrain::setSize(int size)
{
    m_terrainRadius = size/2;
}

GLuint Terrain::getTexId()
{
    return mTexId;
}

bool Terrain::isTerrain()
{
    return true;
}
