#ifndef NPC_H
#define NPC_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <chrono>
#include <thread>

#include "vertex.h"
#include "visualobject.h"
// Oppgave 15 setter større område for bomber å genereres.
class Npc : public VisualObject
{
private:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    float x = 0.0f, xmin= -5.0f, xmax=10.0f, ymin= 0.0f, ymax=5.0f, h=0.1f;

    bool rightDirection = true;

    int path = 0;
    float extent = 0.5;

public:
    Npc();
    Npc(QVector3D Pos);

    void init(GLint matrixUniform) override;
    void draw() override;
    float align(float x, float y);
    float align2(float x);
    void move(float dx, float dy, float dz, float dt) override;
    void npcMove(float dt) override;
    void setPath() override;
    float getSpacing() override;
};

#endif // NPC_H
