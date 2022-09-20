#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

//Implementer en klasse Tetraeder eller en klasse Kube som arver VisualObject.

class Tetrahedron : public VisualObject
{
private:

protected:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    QVector3D mPos;

public:
    Tetrahedron();
    Tetrahedron(QVector3D middlePoint);
    Tetrahedron(std::string filnavn);
    ~Tetrahedron() override;
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);

    void init(GLint matrixUniform) override;
    void draw() override;
    float align(float x, float y);
    void move(float dx, float dy, float dz, float dt) override;
    QVector3D getPlayerPos();
};

#endif // TETRAHEDRON_H
