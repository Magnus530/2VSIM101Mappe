#include "visualobject.h"

VisualObject::VisualObject()
{}

VisualObject::VisualObject(GLuint shaderNum, GLuint id)
{
//    mBBox = new BoundingBox(getPos3D());
}

VisualObject::VisualObject(QVector3D mP, GLint mUniform)
{
//    mBBox = new BoundingBox(getPos3D());
//    mBBox->init(mUniform);
}

VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

void VisualObject::draw2()
{
//    mBBox->draw();
}

glm::mat4 VisualObject::getTransform()
{
    return createTransformMatrix(mGlmPos, mGlmRot, mGlmScale);
}

std::pair<float, float> VisualObject::getPosition2D()
{
    auto col = mPosition.column(3);
    return std::pair<float, float> (col.x(), col.z());
}

QVector3D VisualObject::getPos3D()
{
//    std::cout << "xyz: " << mMatrix.column(3).x() << ", " << mMatrix.column(3).y() << ", " << mMatrix.column(3).z() << "\n";
    return QVector3D{mMatrix.column(3).x(), mMatrix.column(3).y(), mMatrix.column(3).z()};
}

glm::vec3 VisualObject::getGlmPos3D()
{
    return glm::vec3(mMatrix.column(3).x(), mMatrix.column(3).y(), mMatrix.column(3).z());
}

void VisualObject::setPos(QVector3D pos)
{
    mMatrix.setColumn(3, {pos.x(), pos.y(), pos.z(), 1});
}

