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
    int d{0};
    std::vector<float>t{0,0,0,0,0,0,0,0,0,0};
    std::vector<float>c{0,0,0,0,0,0,0,0,0,0};
    //functions
    int findKnotInterval (float x);
    glm::vec3 EvalutaeBSpline(float x);
};

#endif // BSPLINECURVE_H
