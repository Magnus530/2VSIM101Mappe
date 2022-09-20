#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <string>

#include "vertex.h"
#include "texture.h"
#include "Toolbox.h"
#include "camera.h"

struct Triangle
{
    Vertex A;
    Vertex B;
    Vertex C;
};

class BoundingBox;

class VisualObject : public QOpenGLFunctions_4_1_Core
{
protected:
    GLuint mShaderNum = 0;
    GLuint mTexId = 0;

    GLint mMMatrixUniform = 0;
    BoundingBox* mBBox{nullptr};

    float mW = 0, mX = 0, mY = 0, mZ = 0;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mIBO{0};

    GLint mMatrixUniform{0};

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    QMatrix4x4 mPosition;
    QMatrix4x4 mRotation;
    QMatrix4x4 mScale;

    glm::vec3 mGlmPos;
    glm::vec3 mGlmRot;
    glm::vec3 mGlmScale;
    glm::mat4 mModelMatrix{1.f};

public:
    QVector3D mVelocity;
    float mMass = 1.f;
    float mElasticity = 0.5f;

    QVector3D mPos = {0, 0, 0};
    QMatrix4x4 mMatrix;

    GLfloat mAmbientStrength{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f}; // Grey
    GLfloat mLightStrength{10.9f};
    QVector3D mLightColor{0.9f, 0.9f, 0.3f};    //Yellow
    GLfloat mSpecularStrength{10.9f};
    GLint mSpecularExponent{4};
    // Oppgave 10 Prøver å sende inn hvit object color i shader
    QVector3D mObjectColor{1.f,1.f,1.f}; // white

    VisualObject();
    VisualObject(GLuint shaderNum, GLuint id);
    VisualObject(QVector3D mP, GLint mUniform);
    ~VisualObject();

    virtual void init(GLint matrixUniform)=0;
    virtual void draw()=0;
    void draw2();
    void draw(const glm::mat4 &pMat);

    virtual bool isDoor(VisualObject*){return false;}
    virtual bool isLight(VisualObject*){return false;}
    virtual bool isTerrain(VisualObject*){return false;}

    virtual void move(float dt){}
    virtual void move(float x, float y, float z, float dt){}
    virtual void move2(float x, float y, float z, float dt){}
    virtual void npcMove(float dt){}
    virtual void setPath(){}

    virtual GLuint getShaderNum(){return mShaderNum;}
    virtual GLuint getTexId(){return 0;}

    // Used for quadtree.
    std::pair<float, float> getPosition2D();

    QVector3D getPos3D();
    glm::vec3 getGlmPos3D();

    void setPos(QVector3D pos);
    glm::mat4 getTransform();

    virtual void rotate(float dt){}
    virtual void rotate2(int dt, VisualObject* vPtr, Camera* cam){}

    virtual void incrementTrophies(){}
    virtual void incrementBTrophies(){}
    virtual float getTrophies(){return 0;}
    virtual float getBTrophies(){return 0;}
    virtual void setTrophies(float num){}
    virtual void setBTrophies(float num){}
    virtual float getSpacing(){return 0;}
    virtual void getCamPos(QVector3D){}
    virtual bool getIsOpen(){return 0;}
};

#endif // VISUALOBJECT_H
