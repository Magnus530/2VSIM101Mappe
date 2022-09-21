#ifndef HOUSE_H
#define HOUSE_H

#include "visualobject.h"
#include "terrain.h"

class House : public VisualObject
{
private:
    Terrain* tPtr = nullptr;

    float mRadius{0.5};

    float spacing = 4;

public:

    House();
    House(QVector3D middlePoint);
    ~House() override;

    void getTerrain(VisualObject* vPtr);

    void init(GLint matrixUniform) override;
    void draw() override;
    float align(float x, float y);
    float getSpacing() override;

    void construct(QVector3D middlePoint);
    void construct2(QVector3D middlePoint);
};

#endif // HOUSE_H
