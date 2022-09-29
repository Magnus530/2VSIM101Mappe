#include "collision.h"
#include "renderwindow.h"
#include "string"

Collision::Collision()
{}

// Finds the barysentric coordinates for a point p in relation to other points A, B, C.
glm::vec3 Collision::barysentricCoords(glm::vec3 p, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{    
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 normalVec =  glm::cross(AB, AC);
    float totalAreaSquared = glm::length(normalVec);

    glm::vec3 BC = C - B;
    glm::vec3 Bp = p - B;
    glm::vec3 partNormalVec1 = glm::cross(BC, Bp);

    float u = glm::dot(normalVec, partNormalVec1 / totalAreaSquared);

    glm::vec3 CA = A - C;
    glm::vec3 Cp = p - C;

    glm::vec3 partNormalVec2 = glm::cross(CA, Cp);

    float v = glm::dot(normalVec, partNormalVec2 / totalAreaSquared);

    float w = 1 - u - v;

    return glm::vec3(u, v, w);
}

glm::vec3 Collision::normalVec(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 normalVec =  glm::cross(AB, AC);

    return normalVec;
}

// returnerer true hvis punktet er i trekantent
bool Collision::PointIn(glm::vec3&pt, glm::vec3& A, glm::vec3& B, glm::vec3& C)
{
    const glm::vec3 a = A - pt;
    const glm::vec3 b = B - pt;
    const glm::vec3 c = C- pt;

    const glm::vec3 u1 = glm::cross(b, c);
    const glm::vec3 u2 = glm::cross(c, a);
    const glm::vec3 u3 = glm::cross(a, b);

    if (glm::dot(u1, u2) < 0.f) return false;
    if (glm::dot(u1, u3) < 0.f) return false;

    return true;
}

bool Collision::intersect(VisualObject* o1, VisualObject* o2, Contact &hit)
{
    GravitasjonsBall* gBall = static_cast<GravitasjonsBall*>(o1);
    Triangles* mTri = static_cast<Triangles*>(o2);

    for (int fg = 0; fg < mTri->mTriangles.size(); fg++)
    {
        glm::vec3 bc = (*gBall).getGlmPos3D();
        float r = (*gBall).mRadius;

        glm::vec3 A = (*mTri).mTriangles[fg].A.getVertexXYZ();
        glm::vec3 B = (*mTri).mTriangles[fg].B.getVertexXYZ();
        glm::vec3 C = (*mTri).mTriangles[fg].C.getVertexXYZ();

        planePoint = closestPt(bc, A, B, C);
        if (PointIn(planePoint, A, B, C))
        {
            glm::vec3 direction = bc - planePoint;
            float distance = glm::length(direction);

            if(distance <= r)
            {
                hit.A = gBall;
                hit.B = &mTri->mTriangles[fg];

                glm::vec3 hitNormal = direction / distance;

                hit.aContact = bc - hitNormal * r;
                hit.bContact = planePoint;

                hit.depth = r - distance;

                hit.normalVec = hitNormal;

                return true;
            }
        }

        glm::vec3 cp = contactPoint(bc, r, A, B, C);

        glm::vec3 direction = bc - cp;
        float distance = glm::length(direction);

        if(distance <= r)
        {
            hit.A = gBall;
            hit.B = &mTri->mTriangles[fg];

            glm::vec3 hitNormal = direction / distance;

            hit.aContact = hitNormal * r;
            hit.bContact = planePoint;

            hit.depth = r - distance;

            hit.normalVec = hitNormal;

            return true;
        }
    }
    return false;
}

bool Collision::intersect(GravitasjonsBall * gBall, Triangle *tri, Contact &hit)
{

    glm::vec3 bc = (*gBall).getGlmPos3D();
    float r = (*gBall).mRadius;

    glm::vec3 A = tri->A.getVertexXYZ();
    glm::vec3 B = tri->B.getVertexXYZ();
    glm::vec3 C = tri->C.getVertexXYZ();

    planePoint = closestPt(bc, A, B, C);
    if (PointIn(planePoint, A, B, C))
    {
        glm::vec3 direction = bc - planePoint;
        float distance = glm::length(direction);

        if(distance <= r)
        {
            hit.A = gBall;
            hit.B = tri;

            glm::vec3 hitNormal = direction / distance;

            hit.aContact = bc - hitNormal * r;
            hit.bContact = planePoint;

            hit.depth = r - distance;

            hit.normalVec = hitNormal;

            return true;
        }
    }

    glm::vec3 cp = contactPoint(bc, r, A, B, C);

    glm::vec3 direction = bc - cp;
    float distance = glm::length(direction);

    if(distance <= r)
    {
        hit.A = gBall;
        hit.B = tri;

        glm::vec3 hitNormal = direction / distance;

        hit.aContact = hitNormal * r;
        hit.bContact = planePoint;

        hit.depth = r - distance;

        hit.normalVec = hitNormal;

        return true;
    }

    return false;
}

bool Collision::intersect(GravitasjonsBall * gBall, mapTriangle *tri, Contact &hit)
{
    glm::vec3 bc = (*gBall).getGlmPos3D();
    float r = (*gBall).mRadius;

    glm::vec3 A = tri->A.getVertexXYZ();
    glm::vec3 B = tri->B.getVertexXYZ();
    glm::vec3 C = tri->C.getVertexXYZ();

    planePoint = closestPt(bc, A, B, C);
    if (PointIn(planePoint, A, B, C))
    {
        glm::vec3 direction = bc - planePoint;
        float distance = glm::length(direction);

        if(distance <= r)
        {
            hit.A = gBall;
            hit.mapB = tri;

            glm::vec3 hitNormal = direction / distance;

            hit.aContact = bc - hitNormal * r;
            hit.bContact = planePoint;

            hit.depth = r - distance;

            hit.normalVec = hitNormal;

            return true;
        }
    }

    glm::vec3 cp = contactPoint(bc, r, A, B, C);

    glm::vec3 direction = bc - cp;
    float distance = glm::length(direction);

    if(distance <= r)
    {
        hit.A = gBall;
        hit.mapB = tri;

        glm::vec3 hitNormal = direction / distance;

        hit.aContact = hitNormal * r;
        hit.bContact = planePoint;

        hit.depth = r - distance;

        hit.normalVec = hitNormal;

        return true;
    }

    return false;
}

#include "logger.h"

bool Collision::intersect(GravitasjonsBall *A, GravitasjonsBall *B, Contact &hit)
{
    glm::vec3 bc1 = A->getGlmPos3D();
    glm::vec3 bc2 = B->getGlmPos3D();
    float r1 = A->mRadius;
    float r2 = B->mRadius;

    glm::vec3 dir = bc1 - bc2;
    const float dist = glm::length(dir);

    const float radiusSum = r1 + r2;

    if (dist <= radiusSum)
    {
        hit.A = A;
        hit.A1 = B;

        glm::vec3 hitNormal = dir / dist;
        hit.normalVec = hitNormal;

        hit.aContact = bc1 - hitNormal * r1;
        hit.bContact = bc2 + hitNormal * r2;

        hit.depth = glm::length(hit.aContact - hit.bContact);

        return true;
    }

    return false;
}

glm::vec3 Collision::contactPoint(glm::vec3 mP, float radius, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    float mRadius = radius;

    glm::vec3 returnVec;

    glm::vec3 dir1 = B - A;
    glm::vec3 dir2 = C - A;
    glm::vec3 dir3 = C - B;

    glm::vec3 E1 = mP - A;
    glm::vec3 E2 = mP - A;
    glm::vec3 E3 = mP - B;

    float t1 = glm::dot(E1, dir1) / glm::dot(dir1, dir1);
    float t2 = glm::dot(E2, dir2) / glm::dot(dir2, dir2);
    float t3 = glm::dot(E3, dir3) / glm::dot(dir3, dir3);

    t1 = glm::clamp(t1, 0.f, 1.f);
    t2 = glm::clamp(t2, 0.f, 1.f);
    t3 = glm::clamp(t3, 0.f, 1.f);

    glm::vec3 F1 = A + dir1 * t1;
    glm::vec3 F2 = A + dir2 * t2;
    glm::vec3 F3 = B + dir3 * t3;

    glm::vec3 G1 = mP - F1;
    glm::vec3 G2 = mP - F2;
    glm::vec3 G3 = mP - F3;

    float l1 = glm::sqrt(glm::pow(G1.x, 2) + glm::pow(G1.y, 2) + glm::pow(G1.z, 2));
    float l2 = glm::sqrt(glm::pow(G2.x, 2) + glm::pow(G2.y, 2) + glm::pow(G2.z, 2));
    float l3 = glm::sqrt(glm::pow(G3.x, 2) + glm::pow(G3.y, 2) + glm::pow(G3.z, 2));

    if(l1 < l2 && l1 < l3)
    {
        lengthToBall = l1;
        returnVec = F1;
        closestPoint = F1;
    }
    else if (l2 < l1 && l2 < l3)
    {
        lengthToBall = l2;
        returnVec = F2;
        closestPoint = F2;
    }
    else
    {
        lengthToBall = l3;
        returnVec = F3;
        closestPoint = F3;
    }

    return returnVec;
}

glm::vec3 Collision::closestPt(glm::vec3 mP, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    TrianglePlane tPlane;

    tPlane = computePlane(A, B, C);
    float distance = glm::dot(mP, tPlane.normal) - tPlane.d;

    glm::vec3 returnVec = mP - tPlane.normal * distance;

    return returnVec;
}

TrianglePlane Collision::computePlane(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    TrianglePlane p;
    glm::vec3 AB = B-A;
    glm::vec3 AC = C-A;

    p.normal = glm::normalize(glm::cross(AB, AC));
    p.d = glm::dot(p.normal, A);
//    std::cout << p.d << std::endl;
    RenderWindow::Get()->DrawDebugPoint("p2", p.normal * p.d);
    return p;
}
