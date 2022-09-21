#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "visualobject.h"
#include "vertex.h"

class ObjLoader : public VisualObject
{
private:
    float time = 0;

public:
    ObjLoader(std::string filename);
    ObjLoader(QVector3D middlePoint, GLuint shaderNum, GLuint id, std::string filename);
    ~ObjLoader() override;

    void readFile(std::string filename);

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

    GLuint getTexId() override;
    void rotate2(int dt, VisualObject* vPtr, Camera* cam) override;
    void move(float dt) override;
    void move2(float x, float y, float z, float dt) override;
};

#endif // OBJLOADER_H
