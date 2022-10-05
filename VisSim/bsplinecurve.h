#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H
#include "glm/glm.hpp"
#include "vector"
#include "visualobject.h"

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve();
    bool bHasBeinUpdatedOnce{false};

    //variables
    int n{0};
    int d{2};
    float tMin{0.f};
    float tMax{1.f};
    float dt{0.1f};

    std::vector<float> t;
    std::vector<glm::vec3> c;
    //functions
    void firstUpdate(glm::vec3 controlpoint);
    void update(glm::vec3 controlpoint);
    int findKnotInterval (float x);
    glm::vec3 EvalutaeBSpline(float x);
    void init(GLint matrixUniform) override;
    void draw() override;
    void calculateBSpline();
};

#endif // BSPLINECURVE_H
