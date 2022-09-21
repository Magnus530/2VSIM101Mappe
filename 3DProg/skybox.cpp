#include "skybox.h"

Skybox::Skybox()
{}

Skybox::Skybox(GLuint shaderNum, GLuint id)
{
    mShaderNum = shaderNum;
    mTexId = id;

    float size = 10;

    // Lower

    mVertices.push_back(Vertex{-size,-size,size,0,0,1,0,0}); // LSW 0
    mVertices.push_back(Vertex{size,-size,size,0,0,1,1,0}); // LSE 1
    mVertices.push_back(Vertex{size,-size,-size,0,0,1,1,1}); // LNE 2
    mVertices.push_back(Vertex{-size,-size,-size,0,0,1,0,1}); // LNW 3

    // Upper

    mVertices.push_back(Vertex{-size,size,size,0,0,1,0,0}); // USW 4
    mVertices.push_back(Vertex{size,size,size,0,0,1,1,0}); // USE 5
    mVertices.push_back(Vertex{size,size,-size,0,0,1,1,1}); // UNE 6
    mVertices.push_back(Vertex{-size,size,-size,0,0,1,0,1}); // UNW 7

    mIndices =
    {
        // Bottom
        0, 1, 2,
        0, 2, 3,

        // Walls
        0, 1, 5,
        0, 5, 4,

        1, 2, 6,
        1, 6, 5,

        2, 3, 7,
        2, 7, 6,

        3, 0, 4,
        3, 4, 7,

        // Top
        4, 5, 6,
        4, 6, 7

    };

    mMatrix.setToIdentity();
}

void Skybox::init(GLint matrixUniform)
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

    // 1st attribute buffer : vertices
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

    std::string cubemapFaces[6] =
    {
        "../3Dprog22/textures/hund.bmp",
        "../3Dprog22/textures/hund.bmp",
        "../3Dprog22/textures/hund.bmp",
        "../3Dprog22/textures/hund.bmp",
        "../3Dprog22/textures/hund.bmp",
        "../3Dprog22/textures/grass2.bmp"
    };

    cubemapTexture = 0;
    glGenTextures(1, &mTexId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &mColumns, &mRows, &nrChannels, 0);
        if(data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0,
                        GL_RGB,
                        mColumns,
                        mRows,
                        0,
                        GL_RGB,
                        GL_UNSIGNED_BYTE,
                        data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Something is stopping the loading of texture: " << cubemapFaces[i] << "\n";
            stbi_image_free(data);
        }
    }

    glBindVertexArray(0);
}

void Skybox::draw()
{
    initializeOpenGLFunctions();

    glBindVertexArray( mVAO );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexId);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

GLuint Skybox::getTexId()
{
    return mTexId;
}
