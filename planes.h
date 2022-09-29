#ifndef PLANES_H
#define PLANES_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

struct mapPlane
{
    Vertex A;
    Vertex B;
    Vertex C;
    Vertex D;

    std::vector<GLuint> mapIndices;
};

class Planes : public VisualObject
{
private:
    std::vector<mapPlane> mPlanes;
    int n = 0;

public:
    Planes();
    Planes(float height, float xMax, float zMax, float dist, float num, GLuint shaderNum, GLuint id);

    void construct(float height, float xMax, float zMax, float dist, float num);
    void constructPlane(float height, float xMax, float zMax);
    void init(GLint matrixUniform) override;
    void draw() override;

};

#endif // PLANES_H
