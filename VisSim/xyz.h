#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class XYZ : public VisualObject
{
private:
    float scale = 10;

public:
   XYZ();
   XYZ(GLuint shaderNum, GLuint id);

  ~XYZ() override;
  void init(GLint matrixUniform) override;
  void draw() override;

};

#endif // XYZ_H