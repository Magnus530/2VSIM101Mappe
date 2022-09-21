#ifndef COORDREAD_H
#define COORDREAD_H

#include "visualobject.h"
#include <cstddef>
#include <algorithm>

class CoordRead : public VisualObject
{
private:
    int mScale = 4;

public:
    CoordRead();
    CoordRead(std::string fileName, GLuint shaderNum, GLuint id, QVector3D mP);

    void readFile(std::string fileName);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // COORDREAD_H
