#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Cube : public VisualObject
{
private:
    float extent = 1.5;

public:
    Cube();
    Cube(QVector3D mP);
    Cube(QVector3D mP, GLuint shaderNum, GLuint id);
    Cube(QVector3D mP, float frac);

    void construct();
    void construct2(float frac);
    void move(float dt) override;
    void init(GLint matrixUniform) override;
    void draw() override;
    float getSpacing() override;
    GLuint getTexId() override;
};

#endif // CUBE_H
