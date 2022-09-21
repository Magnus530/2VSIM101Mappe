#ifndef LISSAJOUS_H
#define LISSAJOUS_H

//Lag en tilsvarende klasse som arver VisualObject og kan brukes til å representere kurver

#include <fstream>

#include "visualobject.h"
#include "vector3d.h"

class Lissajous : public VisualObject
{
public:
    Lissajous();
    Lissajous(std::string fileName);
    void construct();
    void readFile(std::string fileName);
    void writeFile(std::string fileName);
    void init(GLint shader) override;
    void draw() override;
};

#endif // LISSAJOUS_H
