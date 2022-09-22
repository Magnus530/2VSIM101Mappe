#ifndef COORDREAD_H
#define COORDREAD_H

#include "visualobject.h"
#include <cstddef>
#include <algorithm>

struct mapTriangle
{
    std::string name;
    int id = 0;
    glm::vec3 v0, v1, v2 = glm::vec3{0,0,0};
    std::vector<glm::vec3> inPoints;
    float yAverage;

    int n0, n1, n2 = -1;
};

class CoordRead : public VisualObject
{
private:
    Logger* mLog;

    int mScale = 20;

    float xMax = 0;
    float yMax = 0;
    float zMax = 0;

    float xMin = 0;
    float yMin = 0;
    float zMin = 0;

    std::vector<mapTriangle> mTriangles;
    std::vector<glm::vec3> gridPoints;

//    glm::vec3 botLeft = glm::vec3{0,0,0};
//    glm::vec3 botRight = glm::vec3{0,0,0};
//    glm::vec3 topRight = glm::vec3{0,0,0};
//    glm::vec3 topLeft = glm::vec3{0,0,0};

public:
    CoordRead();
    CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP);

    void readFile(std::string fileName);
    void createGrid(float step);
    void triangulate(std::vector<glm::vec3>, float length, float width);
    std::string nameGen(std::vector<mapTriangle> mTri);

    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // COORDREAD_H
