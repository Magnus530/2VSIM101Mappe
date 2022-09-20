#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <fstream>
#include "vertex.h"
#include "visualobject.h"

//struct Triangle
//{
//    Vertex A;
//    Vertex B;
//    Vertex C;
//};

class Triangles: public VisualObject
{
private:
    float mSize = 1;
    float mNumofSquares = 4;

public:
    std::vector<Triangle> mTriangles;

    Triangles();
    Triangles(std::string fileName, GLuint shaderNum, GLuint id);
    void construct(std::string fileName);
    void readFile(std::string fileName);
    void writeFile(std::string fileName);
    void scale();


    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // TRIANGLE_H
