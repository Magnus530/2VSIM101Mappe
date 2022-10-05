#ifndef GRAVITASJONSBALL_H
#define GRAVITASJONSBALL_H

#include "octahedronball.h"
#include "triangles.h"
#include "bsplinecurve.h"

class GravitasjonsBall : public OctahedronBall
{
public:
    GravitasjonsBall(int n, QVector3D vPos);
    ~GravitasjonsBall() override;
    void init(GLint matrixUniform)override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface);
    BSplineCurve* mBSplineCure;
    float bSplineCuretimer{0.f};

    QVector3D mGravity{0.f, -9.81f, 0.f};
    float mRadius = 1;
    float timeAlive{0};
    float maxTimeAlive{12};

protected:
    VisualObject* triangle_surface;
private:
    QVector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};

#endif // GRAVITASJONSBALL_H
