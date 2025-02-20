/* The code contains logic concluded with assistance from Sten, Hans and Benjamin over discord and at school.
Here are their githubs links:
https://github.com/Hedmark-University-College-SPIM/3Dprog22/tree/StenRL_main, https://github.com/Hedmark-University-College-SPIM/3Dprog22/tree/Benjamin.D,
https://github.com/Hedmark-University-College-SPIM/3Dprog22/tree/Hans,
A for loop in the reset function is inspired by code from: https://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it
*/
#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include "mainwindow.h"
#include "logger.h"
RenderWindow* gRW = nullptr;
RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    gRW = this;
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    //    std::cout << "Coords of a1: " << a1.x() << ", " << a1.y() << ", " << a1.z() << "\n";
}
RenderWindow::~RenderWindow()
{
    gRW = nullptr;
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}
// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    mLogger = Logger::getInstance();

    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    //********************** General OpenGL stuff **********************

    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();
    startOpenGLDebugger();

    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    mShaderProgram[0] = new Shader("../2VSIM101Mappe/plainshader.vert", "../2VSIM101Mappe/plainshader.frag");
    mShaderProgram[1] = new Shader ("../2VSIM101Mappe/textureshader.vert", "../2VSIM101Mappe/textureshader.frag");
    mShaderProgram[2] = new Shader("../2VSIM101Mappe/phongshader.vert", "../2VSIM101Mappe/phongshader.frag");
    mShaderProgram[3] = new Shader("../2VSIM101MappeMappe/skybox.vert", "../2VSIM101Mappe/skybox.frag");

    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //********************** Making the object to be drawn **********************

    //Legg til en variabel som kan holde posisjonen til objektet

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
    //    setupSkyboxShader(3);

    mTexture[0] = new Texture();
    mTexture[1] = new Texture("../2VSIM101Mappe/textures/grass2.bmp");
    mTexture[2] = new Texture("../2VSIM101Mappe/textures/hund.bmp");
    mTexture[3] = new Texture("../2VSIM101Mappe/textures/flower.bmp");

    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4};
    mQuadTree.init(a, b, c, d);
    collision = new Collision();
    hit = new Contact();
    terrain = new Terrain(mShaderProgram[2]->getProgram(), mTexture[1]->id(),
            "../2VSIM101Mappe/textures/terrain3.png");
    if(inDebug == true)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{"xyz", new XYZ{mShaderProgram[0]->getProgram(), mTexture[0]->id()}});
    }
    mMap.insert(std::pair<std::string, VisualObject*>{"gamemode", new GameMode{}});
//    mMap.insert(std::pair<std::string, VisualObject*>{"Triangles", new Triangles("../2VSIM101Mappe/Triangles.txt", mShaderProgram[0]->getProgram(),
//                                                      mTexture[0]->id())});

    mLight = new ObjLoader(QVector3D{100,35,25}, mShaderProgram[1]->getProgram(), mTexture[0]->id(),
    "../2VSIM101Mappe/assets/sun.obj");
    mMap.insert(std::pair<std::string, VisualObject*>{"light", mLight});

    mCoordRead = new CoordRead("../2VSIM101Mappe/Terrains/Steian_2.txt", mShaderProgram[2]->getProgram(), mTexture[1]->id(), QVector3D{0,0,0});
    mMap.insert(std::pair<std::string, VisualObject*>{"coordread", mCoordRead});
    mMap.insert(std::pair<std::string, VisualObject*>{"light", new Light(mShaderProgram[0]->getProgram(), mTexture[0]->id(), QVector3D{0,0,0})});

    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        mQuadTree.insert((*it).second->getPosition2D(), (*it).first, (*it).second);
    }
    mCamera = new Camera();
    mCamera->SetCameraPosition(glm::vec3(0, 10, 10));
    mCamera->SetFar(300);

    mCamera->SetCameraPosition({20,80,70});
    mCamera->SetCameraDirection(-40.f, -50.f);

    for (auto it=mQuadTree.begin(); it!=mQuadTree.end(); it++)
    {
        if((*it)->getShaderNum() == mShaderProgram[1]->getProgram())
        {
            (*it)->init(mMMatrixUniform1);
        }
        else if((*it)->getShaderNum() == mShaderProgram[2]->getProgram())
        {
            (*it)->init(mMMatrixUniform2);
        }
        else
        {
            (*it)->init(mMMatrixUniform0);
        }
    }

    glBindVertexArray(0);       //unbinds any VertexArray - good practice
    if (!mInitialized)
        mInitialized = true;
}
std::string RenderWindow::nameGenerator(std::vector<Cube *> bVec)
{
    std::string tempSize = std::to_string(bVec.size());
    std::string tempS = "b" + tempSize;

    return tempS;
}
std::string RenderWindow::BallNameGenerator(std::vector<GravitasjonsBall *> bVec)
{
    std::string tempSize = std::to_string(bVec.size());
    std::string tempS = "gBall" + tempSize;

    return tempS;
}

std::string RenderWindow::BSplineNameGenerator(std::vector<BSplineCurve *> bVec)
{
    std::string tempSize = std::to_string(bVec.size());
    std::string tempS = "BSpline" + tempSize;

    return tempS;

}
// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    glm::mat4 view =mCamera->GetViewMatrix();
    glm::mat4 project =mCamera->GetProjectionMatrix();
    if (!mInitialized) return;
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();
    timer1 = clock();
    float deltaTime = (float)(timer1 - timer2) / 1000.f;
    inputCheck(deltaTime);
    rainDelayCurrent+=deltaTime;
    if(bRainIsOn&&rainDelayCurrent>rainDelay)
    {
        spawnRain();
        rainDelayCurrent=0;
    }

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //what shader to use
    mCamera->UpdateCamera(deltaTime);
    if (auto it = mMap.find("billboard"); it != mMap.end())
    {
        auto& [key, billB] = *it;
                billB->getCamPos({mCamera->GetPosition().x,mCamera->GetPosition().y,mCamera->GetPosition().z});
    }
    for (auto it=mQuadTree.begin(); it!=mQuadTree.end(); it++)
    {
        if ((*it)->getShaderNum() == mShaderProgram[1]->getProgram())
        {
            glUseProgram(mShaderProgram[1]->getProgram() );
            glUniformMatrix4fv( mVMatrixUniform1, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv( mPMatrixUniform1, 1, GL_FALSE, glm::value_ptr(project));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, (*it)->getTexId());
            (*it)->draw();
        }
        else if ((*it)->getShaderNum() == mShaderProgram[2]->getProgram())
        {
            glUseProgram(mShaderProgram[2]->getProgram() );
            if(mLight)
            {
                glUniformMatrix4fv( mVMatrixUniform2, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv( mPMatrixUniform2, 1, GL_FALSE, glm::value_ptr(project));

                //                std::cout << "lightObject: " << mLight->mMatrix.column(3).x() << ", " << mLight->mMatrix.column(3).y() <<
                //                             ", " << mLight->mMatrix.column(3).z() << " light: " << mLight->getPos3D().x() << ", " <<
                //                             mLight->getPos3D().y() << ", " << mLight->getPos3D().z() << "\n";

                //                mLight->setPos3D(mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).x(), 3);

                glUniform3f(mLightPositionUniform, mLight->getPos3D().x(), mLight->getPos3D().y(), mLight->getPos3D().z());
                //                glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(), mLight->mMatrix.column(3).z());

                glUniform3f(mObjectColorUniform, mLight->mObjectColor.x(), mLight->mObjectColor.y(), mLight->mObjectColor.z());
                glUniform3f(mCameraPositionUniform, mCamera->GetPosition().x, mCamera->GetPosition().y, mCamera->GetPosition().z);
                glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
                glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrength);
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, (*it)->getTexId());
            (*it)->draw();
        }
        else
        {
            glUseProgram(mShaderProgram[0]->getProgram() );
            glUniformMatrix4fv( mVMatrixUniform0, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv( mPMatrixUniform0, 1, GL_FALSE, glm::value_ptr(project));
            glUniform3f(mLightPositionUniform, mLight->getPos3D().x(), mLight->getPos3D().y(), mLight->getPos3D().z());
            (*it)->draw();
        }
    }
    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();
    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
    for(auto it =  mMap.begin(); it != mMap.end(); it++)
    {
        auto& [name, obj] = *it;
        obj->move(deltaTime);
    }
    UpdatePhysics();
    if (inDebug == false)
    {
        if(auto it = mMap.find("light"); it != mMap.end())
        {
            auto& [key, light] = *it;
            light->move(0.01);
        }
        if(auto it = mMap.find("npc"); it != mMap.end())
        {
                auto& [key, npc] = *it;
                npc->npcMove(deltaTime);
        }
        if(auto it = mMap.find("groundnpc"); it != mMap.end())
        {
            if(bNpcHit == false)
            {
                auto& [key, gnpc] = *it;
                gnpc->npcMove(deltaTime);
                float y = terrain->sampleTerrain(mMap["groundnpc"]->getPos3D().x(), mMap["groundnpc"]->getPos3D().z());
                y -= mMap["groundnpc"]->getPos3D().y();
                mMap["groundnpc"]->mMatrix.translate(0, y, 0);
            }
        }
        for (int i = 0; i < mBombs.size(); i++)
        {
            mBombs[i]->move(deltaTime);
        }
    }

    for(int i{0};i<mBSplineCurves.size();i++)
    {
        SplineTime+=deltaTime;
        if(SplineTime>1)
        {
//            SplineTime=0;
//            SplineChangeVec.x++;
//            SplineChangeVec.y++;
//            SplineChangeVec.z++;
//            mBSplineCurves[i]->update(SplineChangeVec);
//            std::cout<<"Updated vertex: x:"<<SplineChangeVec.x<<" y: "<<SplineChangeVec.y<<" z: "<<SplineChangeVec.z<<"\n";
//            mBSplineCurves[i]->init(mMMatrixUniform0);
        }
    }
    for (int i = 0; i < mGBalls.size(); i++)
    {
        mGBalls[i]->timeAlive+=deltaTime;
        if(mGBalls[i]->mBSplineCure->bHasBeinUpdatedOnce == true)
        {
            mGBalls[i]->bSplineCuretimer+=deltaTime;
           if(mGBalls[i]->bSplineCuretimer>0.3)
            {
                mGBalls[i]->bSplineCuretimer=0.f;
                mGBalls[i]->mBSplineCure->update(mGBalls[i]->getGlmPos3D());
                mGBalls[i]->mBSplineCure->init(mMMatrixUniform0);
            }
        }
        if(mGBalls[i]->timeAlive>mGBalls[i]->maxTimeAlive)
                {
                    for(int it=0;it<mBSplineCurves.size();it++)
                    {
                        for(auto it = mMap.begin(); it!= mMap.end();)
                        {
                            if(it->second==mGBalls[i]->mBSplineCure)
                            {
                                mQuadTree.erase((*it).first, (*it).second);
                                it =mMap.erase(it);
                            }else if(it->second==mGBalls[i])
                            {
                                mQuadTree.erase((*it).first, (*it).second);
                                it= mMap.erase(it);

                            }else
                            {
                                it++;
                            }
                        }
                        if(mBSplineCurves[it]==mGBalls[i]->mBSplineCure)
                        {
                            mGBalls[i]->mBSplineCure=nullptr;
                            mBSplineCurves.erase(mBSplineCurves.begin()+it);

                        }
                    }
                    mGBalls.erase(mGBalls.begin()+i);
                }
    }
    timer2 = timer1;
}
void RenderWindow::setCameraSpeed(float value)
{
    mCamSpeed = value;
}
void RenderWindow::setupPlainShader(int shaderIndex)
{
    mPMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mVMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    mMMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
}
void RenderWindow::setupTextureShader(int shaderIndex)
{
    mPMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mVMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    mMMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    mTextureUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}
void RenderWindow::setupPhongShader(int shaderIndex)
{
    mPMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mVMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    mMMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    mLightColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrength" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightStrength" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}
void RenderWindow::setupSkyboxShader(int shaderIndex)
{
    mPMatrixUniform3 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
    mPMatrixUniform3 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    //    mSkyboxUniform = glUniform1i(glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "skybox"), 0);
}
void RenderWindow::reset()
{
    mMainWindow->statusBar()->showMessage("Resetting.");
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        mQuadTree.erase((*it).first, (*it).second);
    }
    for (auto it = mMap.cbegin(), next_it = it; it != mMap.cend(); it = next_it)
    {
        ++next_it;
        mMap.erase(it);
    }
    mTrophies.clear();
    mBTrophies.clear();
    mBombs.clear();
    init();
}
void RenderWindow::setPlayMode()
{
    inDebug = false;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
    reset();
}
void RenderWindow::setDebugMode()
{
    inDebug = true;
    reset();
    if(wFrame == false)
    {
        wFrame = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
    }
    else
    {
        wFrame = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
    }
}
void RenderWindow::winCondition()
{
    QMessageBox mBox;
    if(mMap["gamemode"]->getTrophies() == mTrophies.size())
    {
        mMap["gamemode"]->setTrophies(0);
        mMap["gamemode"]->setBTrophies(0);
        reset();
        mBox.setText("The game is beat congratulations!");
        mBox.exec();
    }
}
void RenderWindow::defeatCondition()
{
    QMessageBox mBox;
    if(mMap["gamemode"]->getBTrophies() == mBTrophies.size())
    {
        mMap["gamemode"]->setTrophies(0);
        mMap["gamemode"]->setBTrophies(0);
        reset();
        mBox.setText("The npc gathers the blue trophies before you gather the red ones. Try again.");
        mBox.exec();
    }
}
void RenderWindow::DrawDebugPoint(std::string name, glm::vec3 pt)
{
    if (auto it = mMap.find(name); it != mMap.end())
    {
        auto& [name, obj] = (*it);
        obj->setPos(QVector3D {pt.x,pt.y,pt.z});
        return;
    }
    VisualObject* o = new Debug(QVector3D {pt.x,pt.y,pt.z});
    o->init(mMMatrixUniform1);
    mQuadTree.insert(o->getPosition2D(), name, o);
    mMap.insert(std::pair<std::string, VisualObject*>{name, o});
}
RenderWindow *RenderWindow::Get()
{
    return gRW;
}
void RenderWindow::inputCheck(float dt)
{
    float moveSpeed = 0.1f;
    if(mKeyInputMap[Qt::Key_W])
    {
        if (inDebug == false && bHit == false)
        {
            mMap["player"]->move2(0.0f, 0.0f, -moveSpeed, 0.0f);
            float y = terrain->sampleTerrain(mMap["player"]->getPos3D().x(), mMap["player"]->getPos3D().z());
            y -= mMap["player"]->getPos3D().y();
            mMap["player"]->mMatrix.translate(0, y, 0);
        }
        else
        {
            mInput.I = true;
            mCamera->MoveForward(mCamSpeed,dt);
            mMainWindow->statusBar()->showMessage(" WWWW");
        }
    }
    if(mKeyInputMap[Qt::Key_A])
    {
        if (inDebug == false && bHit == false)
        {
            mMap["player"]->rotate2(1, mMap["player"], mCamera);
            float y = terrain->sampleTerrain(mMap["player"]->getPos3D().x(), mMap["player"]->getPos3D().z());
            y -= mMap["player"]->getPos3D().y();
            mMap["player"]->mMatrix.translate(0, y, 0);
        }
        else
        {
            mInput.J = true;
            mCamera->MoveRight(-mCamSpeed,dt);
            mMainWindow->statusBar()->showMessage(" AAAA");
        }
    }
    if(mKeyInputMap[Qt::Key_S])
    {
        if (inDebug == false && bHit == false)
        {
            mMap["player"]->move2(0.0f, 0.0f, moveSpeed, 0.0f);
            float y = terrain->sampleTerrain(mMap["player"]->getPos3D().x(), mMap["player"]->getPos3D().z());
            y -= mMap["player"]->getPos3D().y();
            mMap["player"]->mMatrix.translate(0, y, 0);
        }
        else
        {
            mInput.K = true;
            mCamera->MoveForward(-mCamSpeed,dt);
            mMainWindow->statusBar()->showMessage("S");
        }
    }
    if(mKeyInputMap[Qt::Key_D])
    {
        if (inDebug == false && bHit == false)
        {
            mMap["player"]->rotate2(2, mMap["player"], mCamera);
            float y = terrain->sampleTerrain(mMap["player"]->getPos3D().x(), mMap["player"]->getPos3D().z());
            y -= mMap["player"]->getPos3D().y();
            mMap["player"]->mMatrix.translate(0, y, 0);
        }
        else
        {
            mInput.L = true;
            mCamera->MoveRight(mCamSpeed,dt);
            mMainWindow->statusBar()->showMessage(" DDDD");
        }
    }
    if(inDebug == true)
    {
        if(mKeyInputMap[Qt::Key_Q])
        {
            mInput.U = true;
            mCamera->MoveUp(-mCamSpeed,dt);
        }

        if(mKeyInputMap[Qt::Key_E])
        {
            mInput.O = true;
            mCamera->MoveUp(mCamSpeed,dt);
        }
    }
    if(mKeyInputMap[Qt::Key_Z])
    {
        if (mQuadTree.checkInteractCollision(mMap["player"], mMap["door"], 3) == true)
        {
            mMap["door"]->rotate(0.5);
        }
    }
    if(mKeyInputMap[Qt::Key_C])
    {
        mMap["npc"]->setPath();
        mMainWindow->statusBar()->showMessage("Changing npc path.");
    }
    if(mKeyInputMap[Qt::Key_1])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
    }
    if(mKeyInputMap[Qt::Key_2])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
    }
    if(mKeyInputMap[Qt::Key_3])
    {
        if(auto it = mMap.find("coordread"); it != mMap.end())
        {
            CoordRead* cRead = static_cast<CoordRead*>(mMap["coordread"]);
            cRead->renderArray = true;
        }
    }
    if(mKeyInputMap[Qt::Key_4])
    {
        if(auto it = mMap.find("coordread"); it != mMap.end())
        {
            CoordRead* cRead = static_cast<CoordRead*>(mMap["coordread"]);
            cRead->renderArray = false;
        }
    }
    if(mKeyInputMap[Qt::Key_5])
    {
        mCamera->SetCameraPosition({20,80,70});
        mCamera->SetCameraDirection(-40.f, -50.f);
    }

    if(mKeyInputMap[Qt::Key_6])
    {
        mCamera->SetCameraPosition({135,5,75});
        mCamera->SetCameraDirection(-140.f, 0.f);
    }

    if(mKeyInputMap[Qt::Key_7])
    {
        mCamera->SetCameraPosition({100,40,-3});
        mCamera->SetCameraDirection(-260.f, -35.f);
    }

    if(mKeyInputMap[Qt::Key_8])
    {
        if(mCoordRead)
        {
            mCoordRead->init(mMMatrixUniform0);
            mCoordRead->mShaderNum = mShaderProgram[0]->getProgram();
//            mCoordRead->mTexId = mTexture[0]->id();
        }
    }

    if(mKeyInputMap[Qt::Key_9])
    {
        if(auto it = mMap.find("coordread"); it != mMap.end())
        {
            mCoordRead->init(mMMatrixUniform2);
            mCoordRead->mShaderNum = mShaderProgram[2]->getProgram();
            mCoordRead->mTexId = mTexture[1]->id();
        }
    }

    if (mMouseInputMap[Qt::RightButton])
    {
        if(inDebug == true)
        {
            mInput.RMB = true;
            mMainWindow->statusBar()->showMessage(" DDDD");
        }
    }
    if (mMouseInputMap[Qt::LeftButton])
    {
        mInput.LMB = true;
        mMainWindow->statusBar()->showMessage("Left mousebutton");
    }
    if (mMouseInputMap[Qt::MiddleButton])
    {
        mInput.MMB = true;
        mMainWindow->statusBar()->showMessage("middlebutton");
    }
}
void RenderWindow::checkCollision()
{
    float extent = 0.05;
    float bExtent = 1.5;
    trophyCollision(mMap["player"], mTrophies, 0, extent);
    trophyCollision(mMap["groundnpc"], mBTrophies, 1, extent);
    staticCollision(mMap["player"], mMap["cube1"]);
    staticCollision(mMap["player"], mMap["cube2"]);
    staticCollision(mMap["player"], mMap["cube3"]);
    staticCollision(mMap["player"], mMap["cube4"]);
    staticCollision(mMap["player"], mMap["cube5"]);
    staticCollision(mMap["player"], mMap["cube6"]);
    houseCollision(mMap["player"], mMap["house"], mMap["door"]->getIsOpen());
    bombCollision(mMap["player"], mBombs, bExtent/bSize);
    bombCollision(mMap["groundnpc"], mBombs, bExtent/bSize);
}
void RenderWindow::trophyCollision(VisualObject *vPtr1, std::vector<Trophy*> tVec, int tColor, float extent)
{
    if (vPtr1)
    {
        for (int i = 0; i < tVec.size(); i++)
        {
            if (mQuadTree.checkInteractCollision(vPtr1, tVec[i], extent) == true)
            {
                for (auto it = mMap.begin(); it != mMap.end(); it++)
                {
                    if((*it).first != "player")
                    {
                        if(tVec[i]->getCollection() == false)
                        {
                            if((*it).second == tVec[i])
                            {
                                mQuadTree.erase((*it).first, (*it).second);
                                if (tColor == 0)
                                {
                                    mMap["gamemode"]->incrementTrophies();
                                }
                                else
                                {
                                    mMap["gamemode"]->incrementBTrophies();
                                }
                                tVec[i]->setCollection();
                            }
                        }
                    }
                }
            }
        }
    }
}
void RenderWindow::bombCollision(VisualObject *vPtr1, std::vector<Cube*> bVec, float extent)
{
    if(vPtr1)
    {
        for(int i = 0; i < bVec.size(); i++)
        {
            if(mQuadTree.checkBombCollision(vPtr1, bVec[i], extent) == true)
            {
                for(auto it = mMap.begin(); it != mMap.end(); it++)
                {
                    if((*it).first != "player" && (*it).first != "groundnpc")
                    {
                        if((*it).second == bVec[i])
                        {
                            if (vPtr1 == mMap["player"])
                            {
                                bHit = true;
                                mQuadTree.erase((*it).first, (*it).second);
                            }
                            else
                            {
                                bNpcHit = true;
                                mQuadTree.erase((*it).first, (*it).second);
                            }
                        }
                    }
                }
                mMainWindow->statusBar()->showMessage("Interact collision activated");
            }
        }
    }
}
void RenderWindow::staticCollision(VisualObject *vPtr1, VisualObject *vPtr2)
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        float oE = 0.05;
        float step = 0.1;
        if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 0)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().x() > vPtr2->getPos3D().x() + vPtr2->getSpacing() - oE)
            {
                tempVec = {vPtr1->getPos3D().x() + step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                vPtr1->setPos(tempVec);
            }
        }
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 1)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().z() < vPtr2->getPos3D().z() - vPtr2->getSpacing() - oE)
            {
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() - step};
                vPtr1->setPos(tempVec);
            }
        }
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 2)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().x() < vPtr2->getPos3D().x() - vPtr2->getSpacing() + oE)
            {
                tempVec = {vPtr1->getPos3D().x() - step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                vPtr1->setPos(tempVec);
            }
        }
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 3)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().z() > vPtr2->getPos3D().z() + vPtr2->getSpacing() + oE)
            {
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() + step};
                vPtr1->setPos(tempVec);
            }
        }
    }
}
void RenderWindow::houseCollision(VisualObject *vPtr1, VisualObject *vPtr2, bool isOpen)
{
    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        float oE = 0.05;
        float step = 0.1;
        if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 0)
        {
            QVector3D tempVec = {0,0,0};
            if (isOpen == false)
            {
                if (vPtr1->getPos3D().x() > vPtr2->getPos3D().x() + vPtr2->getSpacing() - oE)
                {
                    // Outside
                    tempVec = {vPtr1->getPos3D().x() + step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                    vPtr1->setPos(tempVec);
                }
                else
                {
                    // Inside
                    tempVec = {vPtr1->getPos3D().x() - step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                    vPtr1->setPos(tempVec);
                }
            }
        }
        // Checking north side collision of object from inside and outside.
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 1)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().z() < vPtr2->getPos3D().z() - vPtr2->getSpacing() - oE)
            {
                // Outside
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() - step};
                vPtr1->setPos(tempVec);
            }
            else
            {
                // Inside
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() + step};
                vPtr1->setPos(tempVec);
            }
        }
        // Checking west side collision of object from inside and outside.
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 2)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().x() < vPtr2->getPos3D().x() - vPtr2->getSpacing() + oE)
            {
                // Outside
                tempVec = {vPtr1->getPos3D().x() - step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                vPtr1->setPos(tempVec);
            }
            else
            {
                // Inside
                tempVec = {vPtr1->getPos3D().x() + step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                vPtr1->setPos(tempVec);
            }
        }
        // Checking south side collision of object from inside and outside.
        else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 3)
        {
            QVector3D tempVec = {0,0,0};
            if (vPtr1->getPos3D().z() > vPtr2->getPos3D().z() + vPtr2->getSpacing() + oE)
            {
                // Outside
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() + step};
                vPtr1->setPos(tempVec);
            }
            else
            {
                // Inside
                tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() - step};
                vPtr1->setPos(tempVec);
            }
        }
    }
}
void RenderWindow::spawnBall()
{
    std::string ballName = BallNameGenerator(mGBalls);
    GravitasjonsBall* gravPtr = new GravitasjonsBall(6, {mCamera->GetPosition().x,mCamera->GetPosition().y,mCamera->GetPosition().z});
    mMap.insert(std::pair<std::string, VisualObject*>{ballName, gravPtr});
    mQuadTree.insert(mMap[ballName]->getPosition2D(), ballName, mMap[ballName]);
    mMap[ballName]->init(mMMatrixUniform0);
    spawnBSplineCurve();
    gravPtr->mBSplineCure=mBSplineCurves.back();
    mGBalls.push_back(gravPtr);

}
void RenderWindow::spawnRain()
{
    std::string rainName = BallNameGenerator(mGBalls);
    QVector2D rainPosXZ = GetRandomPosXZ();
    GravitasjonsBall* gravPtr = new GravitasjonsBall(2,{rainPosXZ.x(),45,rainPosXZ.y()});
    mMap.insert(std::pair<std::string, VisualObject*>{rainName,gravPtr});
    mQuadTree.insert(mMap[rainName]->getPosition2D(), rainName, mMap[rainName]);
    mMap[rainName]->init(mMMatrixUniform0);
    mGBalls.push_back(gravPtr);
    spawnBSplineCurve();
    mGBalls.back()->mBSplineCure=mBSplineCurves.back();
}

void RenderWindow::spawnBSplineCurve()
{
    std::string curveName = BSplineNameGenerator(mBSplineCurves);
    BSplineCurve* BSplCrv = new BSplineCurve();
    mMap.insert(std::pair<std::string, VisualObject*>{curveName, BSplCrv});
    mQuadTree.insert(mMap[curveName]->getPosition2D(), curveName, mMap[curveName]);
    mMap[curveName]->init(mMMatrixUniform0);
    mBSplineCurves.push_back(BSplCrv);
}
QVector2D RenderWindow::GetRandomPosXZ()
{
    QVector2D randomPosXZ;
    randomPosXZ.setX(Randomize(mCoordRead->tXMin,mCoordRead->tXMax));
    randomPosXZ.setY(Randomize(mCoordRead->tZMin,mCoordRead->tZMax));
    return(randomPosXZ);
}
void RenderWindow::UpdatePhysics()
{
    if(auto it = mMap.find("Triangles"); it != mMap.end())
    {
        Triangles* tri = static_cast<Triangles*>(mMap["Triangles"]);
        for (int i = 0; i < mGBalls.size(); i++)
        {
            for (int j = 0; j < tri->mTriangles.size(); j++)
            {
                Contact contact;
                if(collision->intersect(mGBalls[i], &tri->mTriangles[j], contact) == true)
                {
                    ResolveContact(contact);
                }
            }
        }
    }
    if(auto it = mMap.find("coordread"); it != mMap.end())
    {
        CoordRead* tri = static_cast<CoordRead*>(mMap["coordread"]);
        for (int i = 0; i < mGBalls.size(); i++)
        {
            for (int j = 0; j < tri->mTriangles.size(); j++)
            {
                Contact contact;
                if(collision->intersect(mGBalls[i], &tri->mTriangles[j], contact) == true)
                {
                    ResolveContact(contact);
                    if(!mGBalls[i]->mBSplineCure->bHasBeinUpdatedOnce)
                    {
                        mGBalls[i]->mBSplineCure->firstUpdate(mGBalls[i]->getGlmPos3D());
                        mGBalls[i]->mBSplineCure->init(mMMatrixUniform0);
                    }
                }
            }
        }
    }
    int numBalls = (int)mGBalls.size();
    for (int i = 0; i < (numBalls - 1); i++)
    {
        for (int j = i + 1; j < numBalls; j++)
        {
            Contact contact;
            if(collision->intersect(mGBalls[i], mGBalls[j], contact))
            {
                ResolveContact(contact);
            }
        }
    }
}
void RenderWindow::ResolveContact(Contact &contact)
{
    auto* sphere = contact.A;
    auto* triangle = contact.mapB;
    const float aInvMass = 1.f / sphere->mMass;
    const float bInvMass = triangle? 0.f : (1.f / contact.A1->mMass);
    const float invMassSum = aInvMass + bInvMass;
    const auto n = contact.normalVec;
    const glm::vec3 velA = glm::vec3(sphere->mVelocity.x(),sphere->mVelocity.y(), sphere->mVelocity.z());
    glm::vec3 velB = glm::vec3(0.f, 0.f, 0.f);
    if (!triangle)
        velB = glm::vec3(contact.A1->mVelocity.x(),contact.A1->mVelocity.y(), contact.A1->mVelocity.z());
    glm::vec3 relVel = velA - velB;
    const float aElasticity = sphere->mElasticity;
    const float bElasticity = triangle? 0.5f : contact.A1->mElasticity;
    const float elasticity = aElasticity * bElasticity;
    float moveAngle = glm::dot(relVel, n);
    float J = -(1.f + elasticity) * moveAngle / invMassSum;
    if (moveAngle >= 0.f) //Om de rører seg bort fra hverandre
        J = 0.f;
    const glm::vec3 vecImpulse = J * n;
    glm::vec3 p = vecImpulse * aInvMass;
    QVector3D result = QVector3D(p.x, p.y, p.z);
    sphere->mVelocity += result;
    if (!triangle)
    {
        auto* sphere2 = contact.A1;
        p = -vecImpulse * bInvMass;
        result = QVector3D(p.x, p.y, p.z);
        sphere2->mVelocity += result;
        QVector3D spherePos2 = sphere2->getPos3D() - QVector3D(contact.normalVec.x, contact.normalVec.y,contact.normalVec.z) * contact.depth;
        sphere2->setPos(spherePos2);
    }
    QVector3D spherePos1 = sphere->getPos3D() + QVector3D(contact.normalVec.x, contact.normalVec.y,contact.normalVec.z) * contact.depth;
    sphere->setPos(spherePos1);
}
//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();
    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();
    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));
    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}
//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar
    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}
//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                // will use VIDEO memory as the source for
                // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                 "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                 "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                 "is not legal for the parameters given to that command. "
                                 "It is also given for commands where combinations of parameters "
                                 "define what the legal parameters are.");
                break;
            }
        }
    }
}
//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);
        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}
void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    mMouseInputMap[event->button()] = true;
}
void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mMouseInputMap[event->button()] = false;
}
void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseInputMap[Qt::MouseButton::RightButton])
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;
        mCamera->ProcessMouseMovement(mMouseXlast,mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
//Legg til interaktiv styring av objektet med piltaster.
//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    if(event->key() == Qt::Key_F)
    {
        spawnRain();
    }
    if(event->key() == Qt::Key_T)
    {
        QVector3D camPos={mCamera->GetPosition().x,mCamera->GetPosition().y,mCamera->GetPosition().z};
        QVector3D camDir={mCamera->GetDirection().x,mCamera->GetDirection().y,mCamera->GetDirection().z};
        std::string ballName = BallNameGenerator(mGBalls);
        GravitasjonsBall* gravPtr = new GravitasjonsBall(6, camPos + camDir * 2.f);
        //                                gravPtr->setPos(camPos);
        gravPtr->mVelocity=camDir * 10.f;
        mMap.insert(std::pair<std::string, VisualObject*>{ballName, gravPtr});
        mQuadTree.insert(mMap[ballName]->getPosition2D(), ballName, mMap[ballName]);
        mMap[ballName]->init(mMMatrixUniform0);
        mGBalls.push_back(gravPtr);
        spawnBSplineCurve();
        mGBalls.back()->mBSplineCure=mBSplineCurves.back();
    }
    mKeyInputMap[event->key()] = true;
}
void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    mKeyInputMap[event->key()] = false;
}
void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    mMainWindow->statusBar()->showMessage("wheel");
    //if RMB, change the speed of the camera
    //    if (mInput.RMB)
    //    {
    //                            if (numDegrees.y() < 1)
    //                                setCameraSpeed(-0.01f);
    //                            if (numDegrees.y() > 1)
    //                                setCameraSpeed(0.01f);
    //                            //    }
}
