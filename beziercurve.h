#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"
#include "vertex.h"

class BezierCurve : public VisualObject
{
private:
    static const int arrSize = 4;

    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

//    Vector3D c[arrSize];
    int d = 3;

public:
    BezierCurve();

    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct2();

    float mix(float a, float b, float t);
    float bezierQuadratic(float A, float B, float C, float t);
    float bezierCubic(float A, float B, float C, float D, float t);
    float bezierQuartic(float A, float B, float C, float D, float E, float t);
};

#endif // BEZIERCURVE_H
