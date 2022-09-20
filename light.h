#ifndef LIGHT_H
#define LIGHT_H

// Oppgave 3

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"


class Light : public VisualObject
{
private:
    float spacing = 0.2;

    float time = 0;

public:

    Light();
    Light(GLuint shaderNum, GLuint id, QVector3D middlePoint);

    bool isLight(VisualObject* mObject) override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;

//    GLfloat mAmbientStrength{0.3f};
//    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f}; // Grey
//    GLfloat mLightStrength{0.7f};
//    QVector3D mLightColor{0.9f, 0.9f, 0.3f};    //Yellow
//    GLfloat mSpecularStrength{0.9f};
//    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
