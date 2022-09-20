// Lagd av Hans Ola Hoftun V2022
#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <random>
#include <QRandomGenerator>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Random
{
public:
    static void Init()
    {}

    static int Int()
    {
        return (int)s_distribution(s_RandomEngine)/(int)std::numeric_limits<uint32_t>::max();
    }
    static float Float()
    {
        return (float)s_distribution(s_RandomEngine)/(float)std::numeric_limits<uint32_t>::max();
    }
    static double Double()
    {
        return (float)s_distribution(s_RandomEngine)/(float)std::numeric_limits<uint32_t>::max();
    }
private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type>s_distribution;
};


static float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return outputMin + (outputMax - outputMin) * ((value - inputMin) / (inputMax - inputMin));
}
static double Randomize(const double& min, const double& max)
{
    return remap(QRandomGenerator::system()->generateDouble(), 0,1,min,max);
}
static glm::mat4 createTransformMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    glm::mat4 M{1.f};
    M = glm::translate(M, pos);
    M = glm::rotate(M,rot.x,glm::vec3(1,0,0));
    M = glm::rotate(M,rot.y,glm::vec3(0,1,0));
    M = glm::rotate(M,rot.z,glm::vec3(0,0,1));
    M = glm::scale(M,scale);

    return M;
}
static glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, const float t)
{
    return a + (b-a)*t;
}
static glm::vec3 QuadraticCurve(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const float t)
{
    glm::vec3 p0 = Lerp(a,b,t);
    glm::vec3 p1 = Lerp(b,c,t);
    return Lerp(p0,p1,t);
}
static glm::vec3 CubicCurve(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t)
{
    glm::vec3 p0 = QuadraticCurve(a,b,c,t);
    glm::vec3 p1 = QuadraticCurve(b,c,d,t);
    return Lerp(p0,p1,t);
}
static double HornersFunction(const double polynomial[], const double& x, const int& n)
{
    double result = polynomial[0];
    for(int i = 1; i < n; i++)
        result = result*x + polynomial[i];
    return result;
}

static glm::vec3 BarycentricCoordinates(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& point)
{
    glm::vec2 p12 = p2 - p1;
    glm::vec2 p13 = p3 - p1;

    glm::vec3 Kryss = glm::cross(glm::vec3{p13.x,0,p13.y}, glm::vec3{p12.x,0,p12.y});
    float areal_123 = glm::length(Kryss);

    glm::vec3 returnVec;

    //u
    glm::vec2 p = p2 - point;
    glm::vec2 q = p3 - point;
    Kryss = glm::cross(glm::vec3{q.x, 0, q.y},glm::vec3{p.x, 0, p.y});
    returnVec.x = Kryss.y / areal_123;

    //v
    p = p3 - point;
    q = p1 - point;
    Kryss = glm::cross(glm::vec3{q.x, 0, q.y},glm::vec3{p.x, 0, p.y});
    returnVec.y = Kryss.y / areal_123;

    //w
    p = p1 - point;
    q = p2 - point;
    Kryss = glm::cross(glm::vec3{q.x,0,q.y}, glm::vec3{p.x, 0, p.y});
    returnVec.z = Kryss.y / areal_123;

    return returnVec;
}

static bool validateBarycentricCoordinates(glm::vec3 barycentric_coord)
{
    float arr[3] {barycentric_coord.x, barycentric_coord.y, barycentric_coord.z};
    for(int i = 0; i < 3; i++)
        if(arr[i] < 0 || arr[i] > 1)
            return false;

    return true;
}


#endif // TOOLBOX_H
