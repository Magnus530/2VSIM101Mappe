#ifndef EXTENTBOX_H
#define EXTENTBOX_H

#include "visualobject.h"

class ExtentBox : public VisualObject
{
private:

public:
    ExtentBox();
    ExtentBox(QVector3D middlePoint);
    ~ExtentBox() override;

    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // EXTENTBOX_H
