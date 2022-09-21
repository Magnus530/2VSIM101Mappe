#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QMatrix4x4>
#include <QVector3D>
#include <vector>
#include <string>

#include "vertex.h"
#include "extentbox.h"

class BoundingBox
{
private:
    ExtentBox* mEBox{nullptr};

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    QVector3D mCentre = {0, 0, 0};
    QVector3D mExtent = {1, 1, 1};

    GLint  mMMatrixUniform0{-1};              //OpenGL reference to the Uniform in the shader program

public:
    QMatrix4x4 mMatrix;

    BoundingBox();
    BoundingBox(QVector3D middlePoint);

    void setCentre(QVector3D tempVec);
    QVector3D getCentre();
    void init(GLint matrixUniform);
    void draw();
};

#endif // BOUNDINGBOX_H
