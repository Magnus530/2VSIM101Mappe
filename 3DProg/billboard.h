#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>

#include "vertex.h"
#include "visualobject.h"

class Billboard : public VisualObject
{
public:
    QVector3D mCamPos = {0,0,0};

    Billboard();
    Billboard(QVector3D mP);
    void construct(QVector3D mP);

    void init(GLint matrixUniform) override;
    void draw() override;
    void draw2(QVector3D pos);

    GLuint getTexId() override;
    void getCamPos(QVector3D camPos) override;
};

#endif // BILLBOARD_H
