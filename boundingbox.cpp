#include "boundingbox.h"

BoundingBox::BoundingBox()
{}

BoundingBox::BoundingBox(QVector3D middlePoint)
{
    mEBox = new ExtentBox(middlePoint);
}

void BoundingBox::setCentre(QVector3D tempVec)
{
    mCentre.setX(tempVec.x());
    mCentre.setY(tempVec.y());
    mCentre.setZ(tempVec.z());
}

QVector3D BoundingBox::getCentre()
{
    return mCentre;
}

void BoundingBox::init(GLint matrixUniform)
{
    mEBox->init(matrixUniform);
}

void BoundingBox::draw()
{
    mEBox->draw();
}
