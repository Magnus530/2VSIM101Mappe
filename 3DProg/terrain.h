#ifndef TERRAIN_H
#define TERRAIN_H

#include "visualobject.h"

// Oppgave 2
class Terrain : public VisualObject
{
public:
    Terrain();
    Terrain(GLuint shaderNum, GLuint id, std::string file);
    ~Terrain();
    void init(GLint matrixUniform) override;
    void draw() override;
    void draw(const glm::mat4 &pMat);
    void generate();
    void toggleShading();
    void setSize(int size);
    GLuint getTexId() override;
    float sampleTerrain(const float &x, const float &z);
    bool isTerrain();

private:
    GLuint mIBO{0};
    int m_terrainRadius = 50;
    float m_yScalar = 2.0f;
    std::string m_curTerrain;
    bool b_FlatShading = false;
    void redraw();
    unsigned char* m_localBuffer;
    int m_width, m_depth;
    void createPoint(int x, int z);
};

#endif // TERRAIN_H
