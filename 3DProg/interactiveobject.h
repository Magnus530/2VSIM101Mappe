#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:

public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // INTERACTIVEOBJECT_H
