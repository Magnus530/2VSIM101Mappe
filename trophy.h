#ifndef TROPHY_H
#define TROPHY_H

#include "visualobject.h"
#include "terrain.h"

// Oppgave 8

class Trophy : public VisualObject
{
private:    
    float mRadius{0.5};

    int collectedTrophies = 0;

    bool collected = false;

public:
    Trophy();
    Trophy(QVector3D middlePoint);
    Trophy(QVector3D middlePoint, int color);

//    Trophy(QVector3D middlePoint, GLint mUniform);

    ~Trophy() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void construct(QVector3D middlePoint);
    void construct2(QVector3D middlePoint, int color);
    float align(float x, float y);
    bool checkPlayerCollision(QVector3D);
    bool getCollection();
    void setCollection();
    QVector3D getTrophyPos();
};

#endif // TROPHY_H
