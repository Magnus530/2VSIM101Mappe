#ifndef COORDREAD_H
#define COORDREAD_H

#include "visualobject.h"

class CoordRead : public VisualObject
{
public:
    CoordRead();

    void readFile(std::string fileName);
    void init(GLint matrixUniform) override;
    void draw() override;

};

#endif // COORDREAD_H
