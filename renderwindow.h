#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <time.h>
#include <QMessageBox>

#include "VisSim/XYZ.h"
#include "3DProg/plane.h"
#include "3DProg/tetrahedron.h"
#include "3DProg/cube.h"
#include "visualobject.h"
#include "3DProg/graph.h"
#include "3DProg/trianglesurface.h"
#include "3DProg/lissajous.h"
#include "3DProg/interactiveobject.h"
#include "camera.h"
#include "VisSim/octahedronball.h"
#include "3DProg/disc.h"
#include "3DProg/visualleastsquaresmethod.h"
#include "3DProg/beziercurve.h"
#include "Quadtree.h"
#include "3DProg/npc.h"
#include "3DProg/trophy.h"
#include "3DProg/player.h"
#include "3DProg/gamemode.h"
#include "3DProg/house.h"
#include "3DProg/door.h"
#include "texture.h"
#include "shader.h"
#include "light.h"
#include "3DProg/terrain.h"
#include "input.h"
#include "VisSim/objloader.h"
#include "3DProg/extentbox.h"
#include "3DProg/billboard.h"
#include "3DProg/skybox.h"
#include "VisSim/triangles.h"
#include "VisSim/collision.h"
#include "VisSim/gravitasjonsball.h"
#include "debug.h"
#include "coordread.h"
#include "VisSim/bsplinecurve.h"
#include "planes.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    /********************************************* WINDOW *********************************************/

    bool mRotate{true};     //Check if triangle should rotate
    void reset();
    void setPlayMode();
    void setDebugMode();
    bool wFrame = false;
    void winCondition();
    void defeatCondition();
    bool inDebug = true;
    bool bRainIsOn{false};

    void DrawDebugPoint(std::string name, glm::vec3 pt);
    static RenderWindow* Get();

private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering

/********************************************* OBJECT *********************************************/

    Collision* collision;
    Contact* hit;
    BSplineCurve* mBSplineC;
    float SplineTime{0};
    glm::vec3 SplineChangeVec{0,0,0};
    CoordRead* mCoordRead;
    float rainDelay{0.0};
    float rainDelayCurrent{0};


    bool bHit = false;
    bool bNpcHit = false;
    float bCounter = 0;
    float bSCounter = 0;
    float bNpcCounter = 0;
    float bSize = 5;
    void spawnBomb();
    void bombCheck(float dt);
    std::string nameGenerator(std::vector<Cube*> bVec);
    std::string BallNameGenerator(std::vector<GravitasjonsBall*> bVec);
    std::string BSplineNameGenerator(std::vector<BSplineCurve*> bVec);
    void inputCheck(float dt);
    void checkCollision();
    void trophyCollision(VisualObject* vPtr1, std::vector<Trophy*> tVec, int tColor, float extent);
    void bombCollision(VisualObject* vPtr1, std::vector<Cube*> bVec, float extent);
    void staticCollision(VisualObject* vPtr1, VisualObject* vPtr2);
    void houseCollision(VisualObject* vPtr1, VisualObject* vPtr2, bool isOpen);
    void spawnBall();
    void spawnRain();
    void spawnBSplineCurve();
    QVector2D GetRandomPosXZ();

    void UpdatePhysics();
    void ResolveContact(Contact& contact);

    std::vector<BSplineCurve*> mBSplineCurves;
    std::vector<GravitasjonsBall*> mGBalls;
    std::unordered_map<std::string, VisualObject*> mMap;
    std::unordered_map<int, bool> mKeyInputMap;
    std::unordered_map<int, bool> mMouseInputMap;
    std::vector<VisualObject*> mObjects;
    std::vector<Trophy*> mTrophies;
    std::vector<Trophy*> mBTrophies;
    std::vector<Cube*> mBombs;
    gsml::QuadTree<std::string, VisualObject*> mQuadTree;
    ObjLoader* mLight{nullptr};
    Plane* mPlane{nullptr};
    Terrain* terrain{nullptr};
    Billboard* mBill{nullptr};

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

/********************************************* CAMERA *********************************************/
    void setCameraSpeed(float value);

    Camera* mCamera{nullptr};
    Camera* mCamera2{nullptr};
    Camera *mCurrentCamera{nullptr};
    float mAspectratio{1.f};

    QVector3D camVec{0,3,4};
    Input mInput;
    float mCamSpeed{1.0f};
    float mCamRotSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};
    int mPlayerXlast{0};
    int mPlayerZlast{0};

/********************************************* TEXTURE *********************************************/

    Texture* mTexture[4]{nullptr}; // How many textures can be held
    Shader* mShaderProgram[3]{nullptr}; // How many textures can be held

    void setupPlainShader(int shaderIndex);
    GLint  mPMatrixUniform0{-1};
    GLint  mVMatrixUniform0{-1};
    GLint  mMMatrixUniform0{-1};              //OpenGL reference to the Uniform in the shader program

    void setupTextureShader(int shaderIndex);
    GLint mMMatrixUniform1{-1};
    GLint mVMatrixUniform1{-1};
    GLint mPMatrixUniform1{-1};
    GLint mTextureUniform1{-1};

    void setupPhongShader(int shaderIndex);
    GLint  mPMatrixUniform2{-1};
    GLint  mVMatrixUniform2{-1};
    GLint  mMMatrixUniform2{-1};

    void setupSkyboxShader(int shaderIndex);
    GLint  mPMatrixUniform3{-1};
    GLint  mVMatrixUniform3{-1};
    GLint  mMMatrixUniform3{-1};

    GLint mSkyboxUniform{-1};

    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform2{-1};

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    /********************************************* DIV *********************************************/

    clock_t timer1{0};
    clock_t timer2{0};

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
