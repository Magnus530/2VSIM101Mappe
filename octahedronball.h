#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include <iostream>
#include <sstream>

#include "vertex.h"
#include "visualobject.h"

class OctahedronBall : public VisualObject
{
private:
    int m_rekursjoner;
    int m_indeks;
    void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void oktaederUnitBall();

public:
    OctahedronBall();
    OctahedronBall(int n, QVector3D mP);
    ~OctahedronBall();
    void init(GLint matrixUniform);
    void draw();
};

#endif // OCTAHEDRONBALL_H
