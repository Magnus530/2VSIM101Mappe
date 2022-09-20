#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>

#include "vertex.h"
#include "visualobject.h"

class Plane : public VisualObject
{
private:
    GLuint mIBO{0};

public:
    Plane();
    Plane(GLuint shaderNum, GLuint id);

    void init(GLint matrixUniform) override;
    void draw() override;

    GLuint getTexId() override;

};

#endif // PLANE_H
