#ifndef DEBUG_H
#define DEBUG_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Debug : public VisualObject
{
public:
    Debug();
    Debug(QVector3D mP);
    void construct();
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // DEBUG_H
