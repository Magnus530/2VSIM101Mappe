#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Player : public VisualObject
{
private:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    QVector3D start = {0,0,0};
    int path = 0;
public:
    Player();
    Player(QVector3D middlePoint);
    ~Player() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    float align(float x, float y);
    void move(float dx, float dy, float dz, float dt) override;
    void move2(float x, float y, float z, float dt) override;
    void construct(QVector3D middlePoint);
    void construct2(QVector3D middlePoint);
    void npcMove(float dt) override;
};

#endif // PLAYER_H
