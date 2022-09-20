#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <fstream>

#include "visualobject.h"
#include "vector3d.h"

class TriangleSurface : public VisualObject
{
private:

public:
   TriangleSurface();
   TriangleSurface(GLuint shaderNum, GLuint id);

   void construct();
   float function(float x, float y);
   float doubleIntegralSimpson(float h, float k, float lx, float ux, float ly, float uy);
   TriangleSurface(std::string fileName);
   ~TriangleSurface() override;
   void readFile(std::string fileName);
   void writeFile(std::string fileName);
   void init(GLint shader) override;
   void draw() override;
   GLuint getTexId() override;
};


#endif // TRIANGLESURFACE_H
