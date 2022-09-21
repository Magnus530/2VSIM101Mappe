#ifndef COORDREAD_H
#define COORDREAD_H

#include "visualobject.h"
#include <cstddef>

class CoordRead : public VisualObject
{
public:
    CoordRead();
    CoordRead(std::string fileName, GLuint shaderNum, GLuint id);

    void readFile(std::string fileName);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // COORDREAD_H
