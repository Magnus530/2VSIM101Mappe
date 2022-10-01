#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"
#include "triangles.h"
#include "Toolbox.h"
#include "gravitasjonsball.h"
#include "coordread.h"
#include "planes.h"

struct TrianglePlane
{
    glm::vec3 normal;
    float d;
};

struct Contact
{
    GravitasjonsBall* A{nullptr};
    GravitasjonsBall* A1{nullptr};
    Triangle* B{nullptr};
    mapTriangle* mapB{nullptr};

    glm::vec3 normalVec{0.f, 1.f, 0.f};
    float depth = 0.f;

    glm::vec3 aContact{0.f, 0.f, 0.f};
    glm::vec3 bContact{0.f, 0.f, 0.f};
};

class Collision
{
private:
    float lengthToBall = 0;
    glm::vec3 closestPoint;

public:
    glm::vec3 planePoint;
    QVector3D bCoord;

    Collision();
    glm::vec3 barysentricCoords(glm::vec3 p, glm::vec3 A, glm::vec3 B, glm::vec3 C);
    glm::vec3 normalVec(glm::vec3 A, glm::vec3 B, glm::vec3 C);
    bool intersect(VisualObject* A, VisualObject* B, Contact& hit);
    bool Planesintersect(mapPlane* p, Triangle* tri, Contact& hit);
    bool intersect(GravitasjonsBall* A, Triangle* B, Contact& hit);
    bool intersect(GravitasjonsBall* A, mapTriangle* B, Contact& hit);
    bool intersect(GravitasjonsBall* A, GravitasjonsBall* B, Contact& hit);

    glm::vec3 contactPoint(glm::vec3 mP, float radius, glm::vec3 A, glm::vec3 B, glm::vec3 C);
    glm::vec3 closestPt(glm::vec3 mP, glm::vec3 A, glm::vec3 B, glm::vec3 C);

    TrianglePlane computePlane(glm::vec3 A, glm::vec3 B, glm::vec3 C);
    float computeDistance();
    bool PointIn(glm::vec3&pt, glm::vec3& A, glm::vec3& B, glm::vec3& C);
};

#endif // COLLISION_H
