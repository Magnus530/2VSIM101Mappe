#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H
#include "glm/glm.hpp"
#include "vector"

class BSplineCurve
{
public:
    BSplineCurve();

    //variables
    int n{0};
    int d{2};
    float tMin{0.f};
    float tMax{0.f};

    std::vector<float>t{0,0,0,1,2,2,2};
    std::vector<glm::vec3>c{{0,0,0},{0,0,0},{0,0,0}};
    //functions
    void update();
    int findKnotInterval (float x);
    glm::vec3 EvalutaeBSpline(float x);
};

#endif // BSPLINECURVE_H
