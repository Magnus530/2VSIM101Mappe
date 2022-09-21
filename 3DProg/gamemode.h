#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "visualobject.h"

class GameMode : public VisualObject
{
private:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    float mRadius{0.5};

    int collectedTrophies = 0;
    int collectedBTrophies = 0;

public:
    GameMode();

    void init(GLint matrixUniform) override;
    void draw() override;
    void incrementTrophies() override;
    void incrementBTrophies() override;
    float getTrophies() override;
    float getBTrophies() override;
    void setTrophies(float num) override;
    void setBTrophies(float num) override;
};

#endif // GAMEMODE_H
