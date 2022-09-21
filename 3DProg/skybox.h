#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>

#include "vertex.h"
#include "visualobject.h"
#include "stb_image/stb_image.h"

class Skybox : public VisualObject
{
private:
    unsigned int cubemapTexture = 0;

public:
    int mColumns, mRows, nrChannels;

    Skybox();
    Skybox(GLuint shaderNum, GLuint id);

    void init(GLint matrixUniform) override;
    void draw() override;

    GLuint getTexId() override;
};

#endif // SKYBOX_H
