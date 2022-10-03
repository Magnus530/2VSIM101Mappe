#ifndef COORDREAD_H
#define COORDREAD_H

#include "visualobject.h"
#include <cstddef>
#include <algorithm>
#include <thread>
#include <future>
#include <functional>
#include <atomic>
#include <mutex>
#include <numeric>
#include <fstream>
#include <string>
#include <vector>

struct mapSquare
{
    int id = 0;
    glm::vec3 v0, v1, v2, v3 = glm::vec3{0,0,0};
    glm::vec3 midPoint = glm::vec3{0,0,0};
    std::vector<glm::vec3> inPoints;
};

struct mapTriangle
{
    std::string name;
    int id = 0;
    glm::vec3 v0, v1, v2 = glm::vec3{0,0,0};
    std::vector<glm::vec3> inPoints;
    float yAverage = 0;

    int n0, n1, n2 = -1;

    Vertex A;
    Vertex B;
    Vertex C;
};

class CoordRead : public VisualObject
{
private:
    float sTNumBot = 0.05f;
    float sTNumTop = - 0.05f;

    int mScale = 9;
    int step = 5;

    float xMax = 0;
    float yMax = 0;
    float zMax = 0;

    float xMin = 0;
    float yMin = 0;
    float zMin = 0;

    std::vector<mapSquare> mSquares;

    std::vector<glm::vec3> gridPoints;

public:
    float lXMax = 0;
    float lZMax = 0;
    bool renderArray = false;

    float tXMin;
    float tXMax;
    float tZMin;
    float tZMax;

    std::vector<mapTriangle> mTriangles;
    CoordRead();
    CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP);

    void readFile(std::string fileName);
    void createGrid(float step);
    void createMidGrid(float step);
    void createSquare(float length, float width);
    void triangulate(std::vector<glm::vec3> gridPoints, int length, int width);
    void pointInsert();
    bool boundaryCheck(glm::vec3 vertVec, mapSquare mS);
    float average(float x, float y);
    void averageCalc();
    std::string nameGen(std::vector<mapTriangle> mTri);
    void writeAverage(std::string fileName);
    void readAverage(std::string fileName);

    void init(GLint matrixUniform) override;
    void draw() override;
    GLuint getTexId() override;
};

#endif // COORDREAD_H
