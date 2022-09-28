#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H
#include "glm/glm.hpp"
#include "vector"
#include "visualobject.h"

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve(GLuint shaderNum, GLuint id, glm::vec3 pos1,glm::vec3 pos2,glm::vec3 pos3);

    //variables
    int n{0};
    int d{2};
    float tMin{0.f};
    float tMax{2.f};
    float dt{0.1f};

    std::vector<float>t{0,0,0,2,2,2};
    std::vector<glm::vec3>c;
    //functions
    void update();
    int findKnotInterval (float x);
    glm::vec3 EvalutaeBSpline(float x);
    void init(GLint matrixUniform) override;
    void draw() override;


};

#endif // BSPLINECURVE_H
