#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Graph : public VisualObject
{
private:
    float mX;
    float mY;
    float mZ;

public:
    Graph();
    Graph(float x, float y, float z);
    Vertex assignCoords(float& x, float& y, float& z);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
};

#endif // GRAPH_H
