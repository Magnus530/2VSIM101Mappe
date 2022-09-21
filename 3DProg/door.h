#ifndef DOOR_H
#define DOOR_H

#include "visualobject.h"

class Door : public VisualObject
{
private:
    GLuint mIBO{0};

    bool isOpen = false;

    float mAngle = {0};
    float mSpeed = {0};

public:
    Door();
    Door(QVector3D middlePoint);

    void init(GLint matrixUniform) override;
    void draw() override;
    bool isDoor(VisualObject* ptr) override;
    void rotate(float dt) override;
    float align(float x, float y);
    void construct(QVector3D middlePoint);
    void construct2(QVector3D middlePoint);
    bool getIsOpen() override;
};

#endif // DOOR_H
