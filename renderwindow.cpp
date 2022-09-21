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

    mShaderProgram[0] = new Shader("../2VSIM101/plainshader.vert", "../2VSIM101/plainshader.frag");
    mShaderProgram[1] = new Shader ("../2VSIM101/textureshader.vert", "../2VSIM101/textureshader.frag");
    mShaderProgram[2] = new Shader("../2VSIM101/phongshader.vert", "../2VSIM101/phongshader.frag");
    mShaderProgram[3] = new Shader("../2VSIM101/skybox.vert", "../2VSIM101/skybox.frag");

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
    mTexture[1] = new Texture("../2VSIM101/textures/grass2.bmp");
    mTexture[2] = new Texture("../2VSIM101/textures/hund.bmp");
    mTexture[3] = new Texture("../2VSIM101/textures/flower.bmp");

    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4};
    mQuadTree.init(a, b, c, d);


    collision = new Collision();
    hit = new Contact();

    //    mLight2 = new Light{mShaderProgram[0]->getProgram(), mTexture[0]->id(), QVector3D{0,10,0}};
    mLight = new ObjLoader(QVector3D{0,20,0}, mShaderProgram[1]->getProgram(), mTexture[0]->id(),
            "../Eksamen-3Dprog-2022/assets/sun.obj");
    //    mPlane = new Plane{mShaderProgram[2]->getProgram(), mTexture[2]->id()};
    terrain = new Terrain(mShaderProgram[2]->getProgram(), mTexture[1]->id(),
            "../Eksamen-3Dprog-2022/textures/terrain3.png");

    if(inDebug == true)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{"xyz", new XYZ{mShaderProgram[0]->getProgram(), mTexture[0]->id()}});
    }

    //    mMap.insert(std::pair<std::string, VisualObject*>{"triangle", new TriangleSurface{mShaderProgram[0]->getProgram(), mTexture[0]->id()}});

    //    mMap.insert(std::pair<std::string, VisualObject*>{"plane", new Plane{mShaderProgram[2]->getProgram(), mTexture[2]->id()}});
    //    mMap.insert(std::pair<std::string, VisualObject*>{"cube1", new Cube(QVector3D{15,2,-5}, mShaderProgram[2]->getProgram(), mTexture[0]->id())});

    mMap.insert(std::pair<std::string, VisualObject*>{"gamemode", new GameMode{}});

    //    mMap.insert(std::pair<std::string, VisualObject*>{"light", mLight});
    //    mMap.insert(std::pair<std::string, VisualObject*>{"obj", new ObjLoader(QVector3D{-9,1,-23}, mShaderProgram[1]->getProgram(),
    //                                                      mTexture[1]->id(), "../Eksamen-3Dprog-2022/assets/test.obj")});

    //    mMap.insert(std::pair<std::string, VisualObject*>{"terrain", new Terrain(mShaderProgram[2]->getProgram(), mTexture[1]->id(),
    //                                                      "../Eksamen-3Dprog-2022/textures/terrain3.png")});

    mMap.insert(std::pair<std::string, VisualObject*>{"Triangles", new Triangles("../2VSIM101/Triangles.txt", mShaderProgram[0]->getProgram(),
                                                      mTexture[0]->id())});

    //      mMap.insert(std::pair<std::string, VisualObject*>{"oball", new OctahedronBall(6, QVector3D {0,4,0})});

    for (auto it = mMap.begin(); it != mMap.end(); it++)
    {
        mQuadTree.insert((*it).second->getPosition2D(), (*it).first, (*it).second);
    }

    mCamera = new Camera();
    //    mCamera2 = new Camera();
    //    mCamera2->init(mPMatrixUniform0, mVMatrixUniform0);
    //    mCamera->lookAt(QVector3D{0,0,0}, QVector3D{0, 1, 0});
    //    mCamera->setPosition(mMap["player"]->getPos3D() + QVector3D{0, 4, 8});
    //    mCamera2->lookAt(QVector3D{0,2,1}, QVector3D{0,0,0}, QVector3D{0, 1, 0});
    //    mCamera->perspective(60, (float)width() / (float)height(), 0.1, 1000.0);
    mCamera->SetCameraPosition(glm::vec3(0, 10, 10));
    //    mCamera->pitch(50);
    mCamera->SetFar(300);


    //    QVector3D a1 = BarysentricCoords(QVector3D{1.5,1.5,0}, QVector3D{0,1,0}, QVector3D{1.5,0,0}, QVector3D{2.5,1,0});

    for (auto it=mQuadTree.begin(); it!=mQuadTree.end(); it++)
    {
        if((*it)->getShaderNum() == mShaderProgram[1]->getProgram())
        {
            //            mCamera.init(mPMatrixUniform, mVMatrixUniform1);
            (*it)->init(mMMatrixUniform1);
        }
        else if((*it)->getShaderNum() == mShaderProgram[2]->getProgram())
        {
            //            mCamera.init(mPMatrixUniform2, mVMatrixUniform2);
            (*it)->init(mMMatrixUniform2);
        }
        //        else if((*it)->getShaderNum() == mShaderProgram[3]->getProgram())
        //        {
        ////            mCamera.init(mPMatrixUniform2, mVMatrixUniform2);
        //            (*it)->init(mMMatrixUniform3);
        //        }
        else
        {
            //            mCamera.init(mPMatrixUniform0, mVMatrixUniform0);
            (*it)->init(mMMatrixUniform0);
        }
    }

    //    mPlane->init(mMMatrixUniform2);

    //    spawnBomb();

    glBindVertexArray(0);       //unbinds any VertexArray - good practice

    if (!mInitialized)
        mInitialized = true;
}

// Oppgave 7
void RenderWindow::spawnBomb()
{
    std::string bombName = nameGenerator(mBombs);
    Cube* bombPtr = new Cube(mMap["npc"]->getPos3D(), bSize);
    mMap.insert(std::pair<std::string, VisualObject*>{bombName, bombPtr});
    mQuadTree.insert(mMap[bombName]->getPosition2D(), bombName, mMap[bombName]);
    mMap[bombName]->init(mMMatrixUniform0);
    mBombs.push_back(bombPtr);
    //        std::cout << "bombpos: " << mMap["bomb1"]->getPos3D().x() << ", " << mMap["bomb1"]->getPos3D().y() << ", " << mMap["bomb1"]->getPos3D().z() << "\n";
}

// Oppgave 7 og 9
void RenderWindow::bombCheck(float dt)
{
    float twoSec = 6;

    bSCounter += 1/dt;

    if (bSCounter >= twoSec)
    {
        spawnBomb();
        bSCounter = 0;
    }

    if (bHit == true)
    {
        bCounter += 1/dt;
    }

    if (bCounter >= twoSec)
    {
        bHit = false;
        bCounter = 0;
    }

    if (bNpcHit == true)
    {
        bNpcCounter += 1/dt;
    }

    if (bNpcCounter >= twoSec)
    {
        bNpcHit = false;
        bNpcCounter = 0;
    }
}

// Oppgave 7 navn til bomber.
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
    //    bombCheck(deltaTime);

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //what shader to use
    mCamera->UpdateCamera(deltaTime);
    if(inDebug == false)
    {
        if (mMap["house"] && mMap["player"] && mQuadTree.checkInteractCollision(mMap["player"], mMap["house"],
                                                                                mMap["house"]->getSpacing()) == true)
        {
            mCamera->SetCameraPosition(glm::vec3{-8, 1, -16});
        }
        else
        {
            //            std::cout << "mCamara pos: " << mCamera->getPos3D().x() << ", " << mCamera->getPos3D().y() << ", " <<
            //                         mCamera->getPos3D().z() << "\n";

            //            mCamera->setPosition(mMap["player"]->getPos3D() + camVec);
            //            mCamera->update();
        }
    }

    // Oppgave 11
    if (auto it = mMap.find("billboard"); it != mMap.end())
    {
        auto& [key, billB] = *it;
                billB->getCamPos({mCamera->GetPosition().x,mCamera->GetPosition().y,mCamera->GetPosition().z});
    }

                // When player collides with trophy
                //    checkCollision();

                //     Draw objects
                for (auto it=mQuadTree.begin(); it!=mQuadTree.end(); it++)
        {
            //        std::cout << "shadernum: " << (*it)->getShaderNum() << " shaderprogram num: " << mShaderProgram[1]->getProgram() << "\n";
            if ((*it)->getShaderNum() == mShaderProgram[1]->getProgram())
            {
                glUseProgram(mShaderProgram[1]->getProgram() );

                glUniformMatrix4fv( mVMatrixUniform1, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv( mPMatrixUniform1, 1, GL_FALSE, glm::value_ptr(project));

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, (*it)->getTexId());

                (*it)->draw();
            }
            // Oppgave 2 og oppgave 3
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
            //        else if((*it)->getShaderNum() == mShaderProgram[3]->getProgram())
            //        {
            //            glUseProgram(mShaderProgram[3]->getProgram() );

            //            QMatrix4x4 mVMat;
            //            QMatrix4x4 mPMat;
            //            mVMat.setToIdentity();
            //            mPMat.setToIdentity();
            //            // sjekk vektor senere
            ////            mVMat = QMatrix4x4(QMatrix3x3(mCamera->lookAt(QVector3D{0,5,0}, QVector3D{0,0,0}, QVector3D{0,1,0})));
            ////            mPMat = mCamera->perspective(45/M_PI, (float)mMap["skybox"]->mColumns / mMap["skybox"]->mRows, 0.1f);

            //            glUniformMatrix4fv( mVMatrixUniform3, 1, GL_FALSE, mCamera->mVMatrix.constData());
            //            glUniformMatrix4fv( mPMatrixUniform3, 1, GL_FALSE, mCamera->mPMatrix.constData());

            //            glDepthFunc(GL_LEQUAL);

            ////            glActiveTexture(GL_TEXTURE0);
            ////            glBindTexture(GL_TEXTURE_2D, (*it)->getTexId());
            //        }
            else
            {
                //            std::cout << "in else " << mShaderProgram[0]->getProgram() << " TexId: " << (*it)->getTexId() << "\n";
                glUseProgram(mShaderProgram[0]->getProgram() );


                glUniformMatrix4fv( mVMatrixUniform0, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv( mPMatrixUniform0, 1, GL_FALSE, glm::value_ptr(project));

                if((*it) == mBill)
                {
//                    mBill->draw2(mCamera->GetPosition());
                }
                else
                {
                    (*it)->draw();
                }
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

                    // Oppgave 6, 13 og 3
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

                                // Oppgave 9
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

                            //    mMap["disc"]->move(1);

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

                        // Oppgave 2
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

                        // Oppgave 14
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

                        // Oppgave 12 og oppgave 9 når spiller samler røde trofeer kalles denne if sjekken.
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

                        // Oppgave 12 og oppgave 9 når npc samler blå trofeer kalles denne if sjekken.
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

                        /* Oppgave 4 bytte om A og D funksjonalitet til å rotere.
 Oppgave 5 a og b, få kamera til å rotere etter spillers rotasjon og
 få kamera til å styres med WASD i editor.
Oppgave 16*/
                        void RenderWindow::inputCheck(float dt)
                        {
                            float moveSpeed = 0.1f;

                            if(mKeyInputMap[Qt::Key_W])
                            {
                                if (inDebug == false && bHit == false)
                                {
                                    //            mCamera->setSpeed(-mCamSpeed);
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
                                    //            camVec.setX(camVec.x() + sin(M_PI));
                                    //            camVec.setX(camVec.z() + sin(M_PI));
                                    //            if(camVec.x() < 4.2)
                                    //            {
                                    //            }
                                    //            else
                                    //            {
                                    //                camVec.setX(camVec.x() - 1.f);
                                    //                camVec.setZ(camVec.z() - 0.30f);
                                    //            }
                                    //            std::cout << "Camvec from A: " << camVec.x() << ", " << camVec.y() << ", " << camVec.z() << "\n";
                                    //            mCamera->moveRight(mCamRotSpeed*5);
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
                                    ////            camVec.setX(camVec.x() - 0.15f);
                                    //            camVec.setX(camVec.x() - sin(M_PI));
                                    //            camVec.setX(camVec.z() - sin(M_PI));


                                    //            std::cout << "Camvec from D: " << camVec.x() << ", " << camVec.y() << ", " << camVec.z() << "\n";
                                    //            mCamera->moveRight(-mCamRotSpeed*5);
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

                            // Player and trophy collision.
                            trophyCollision(mMap["player"], mTrophies, 0, extent);
                            trophyCollision(mMap["groundnpc"], mBTrophies, 1, extent);
                            // Collision between two objects.

                            // Oppgave 10
                            staticCollision(mMap["player"], mMap["cube1"]);
                            staticCollision(mMap["player"], mMap["cube2"]);
                            staticCollision(mMap["player"], mMap["cube3"]);
                            staticCollision(mMap["player"], mMap["cube4"]);
                            staticCollision(mMap["player"], mMap["cube5"]);
                            staticCollision(mMap["player"], mMap["cube6"]);

                            // Collision between object and house.
                            houseCollision(mMap["player"], mMap["house"], mMap["door"]->getIsOpen());
                            // Player and npc collision.
                            //    interactCollision(mMap["player"], mMap["npc"], mMap["npc"]->getSpacing());

                            // Player and bomb collision
                            bombCollision(mMap["player"], mBombs, bExtent/bSize);
                            // Ground npc and bomb collision Oppgave 9
                            bombCollision(mMap["groundnpc"], mBombs, bExtent/bSize);
                        }

                        // Oppgave 9 sjekk for blå trofe.
                        void RenderWindow::trophyCollision(VisualObject *vPtr1, std::vector<Trophy*> tVec, int tColor, float extent)
                        {
                            // Interact collision
                            if (vPtr1)
                            {
                                for (int i = 0; i < tVec.size(); i++)
                                {
                                    //            std::cout << "i " << i << " , playerPos: " << mMap["player"]->getPos3D().x() << ", " << mMap["player"]->getPos3D().z() << "\n" <<
                                    //                "trophypos: " << mTrophies[i]->getPos3D().x() << ", " << mTrophies[i]->getPos3D().z() <<
                                    //                "\n" << "bool: " << mQuadTree.checkCollision(mMap["player"], mTrophies[i], extent) << "\n";
                                    if (mQuadTree.checkInteractCollision(vPtr1, tVec[i], extent) == true)
                                    {
                                        for (auto it = mMap.begin(); it != mMap.end(); it++)
                                        {
                                            //                    std::cout << "name: " << (*it).first << " ptr: " << (*it).second << "\n";

                                            if((*it).first != "player")
                                            {
                                                if(tVec[i]->getCollection() == false)
                                                {
                                                    //                            std::cout << "it.name: " << (*it).first << " it.ptr: " << (*it).second << " it.second x: "
                                                    //                                      << (*it).second->getPos3D().x() << " trophy x: " << mTrophies[i]->getTrophyPos().x() << "\n";
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

                        // Oppgave 7 og oppgave 9 sjekk om groundnpc treffes av bombe.
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

                        // Oppgave 16
                        void RenderWindow::staticCollision(VisualObject *vPtr1, VisualObject *vPtr2)
                        {
                            // Static collision
                            for (auto it = mMap.begin(); it != mMap.end(); it++)
                            {
                                float oE = 0.05;
                                float step = 0.1;

                                //        std::cout << "player x: " << mMap["player"]->getPos3D().x() << ", house x - extent: " <<
                                //                     mMap["house"]->getPos3D().x() - mMap["house"]->getSpacing() << "\n";

                                // Checking east side collision of object from outside.
                                if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 0)
                                {
                                    QVector3D tempVec = {0,0,0};

                                    if (vPtr1->getPos3D().x() > vPtr2->getPos3D().x() + vPtr2->getSpacing() - oE)
                                    {
                                        // Outside
                                        tempVec = {vPtr1->getPos3D().x() + step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                                        vPtr1->setPos(tempVec);
                                    }
                                }
                                // Checking north side collision of object from outside.
                                else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 1)
                                {
                                    QVector3D tempVec = {0,0,0};

                                    if (vPtr1->getPos3D().z() < vPtr2->getPos3D().z() - vPtr2->getSpacing() - oE)
                                    {
                                        // Outside
                                        tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() - step};
                                        vPtr1->setPos(tempVec);
                                    }
                                }
                                // Checking west side collision of object from outside.
                                else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 2)
                                {
                                    QVector3D tempVec = {0,0,0};

                                    if (vPtr1->getPos3D().x() < vPtr2->getPos3D().x() - vPtr2->getSpacing() + oE)
                                    {
                                        // Outside
                                        tempVec = {vPtr1->getPos3D().x() - step, vPtr1->getPos3D().y(), vPtr1->getPos3D().z()};
                                        vPtr1->setPos(tempVec);
                                    }
                                }
                                // Checking south side collision of object from outside.
                                else if (mQuadTree.checkStaticCollision(vPtr1, vPtr2, vPtr2->getSpacing()) == 3)
                                {
                                    QVector3D tempVec = {0,0,0};

                                    if (vPtr1->getPos3D().z() > vPtr2->getPos3D().z() + vPtr2->getSpacing() + oE)
                                    {
                                        // Outside
                                        tempVec = {vPtr1->getPos3D().x(), vPtr1->getPos3D().y(), vPtr1->getPos3D().z() + step};
                                        vPtr1->setPos(tempVec);
                                    }
                                }
                            }
                        }

                        void RenderWindow::houseCollision(VisualObject *vPtr1, VisualObject *vPtr2, bool isOpen)
                        {
                            // Static collision
                            for (auto it = mMap.begin(); it != mMap.end(); it++)
                            {
                                float oE = 0.05;
                                float step = 0.1;

                                //                std::cout << "player x: " << mMap["player"]->getPos3D().x() << ", house x + extent: " <<
                                //                             mMap["house"]->getPos3D().x() + mMap["house"]->getSpacing() << "\n";

                                // Checking east side collision of object from inside and outside.
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
                            mGBalls.push_back(gravPtr);
                        }

                        void RenderWindow::UpdatePhysics()
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
                            auto* triangle = contact.B;

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
                                spawnBall();
                            }
                            if(event->key() == Qt::Key_T)
                            {
                                QVector3D camPos={mCamera->GetPosition().x,mCamera->GetPosition().y,mCamera->GetPosition().z};
                                QVector3D camDir={mCamera->GetDirection().x,mCamera->GetDirection().y,mCamera->GetDirection().z};

                                std::string ballName = BallNameGenerator(mGBalls);
                                GravitasjonsBall* gravPtr = new GravitasjonsBall(6, camPos + camDir * 2.f);

//                                gravPtr->setPos(camPos);
                                gravPtr->mVelocity=camPos + camDir * 100.f;

                                mMap.insert(std::pair<std::string, VisualObject*>{ballName, gravPtr});
                                mQuadTree.insert(mMap[ballName]->getPosition2D(), ballName, mMap[ballName]);

                                mMap[ballName]->init(mMMatrixUniform0);
                                mGBalls.push_back(gravPtr);
                            }

                            mKeyInputMap[event->key()] = true;
                        }

                        void RenderWindow::keyReleaseEvent(QKeyEvent *event)
                        {
//                            mCamera->SetCameraSpeed(0.f);
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
