#ifndef VISUALLEASTSQUARESMETHOD_H
#define VISUALLEASTSQUARESMETHOD_H

#include "visualobject.h"

class visualLeastSquaresMethod : public VisualObject
{

protected:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    float mRadius{0.5};

public:
    visualLeastSquaresMethod();
    ~visualLeastSquaresMethod() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();

};

#endif // VISUALLEASTSQUARESMETHOD_H
