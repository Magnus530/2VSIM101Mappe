QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    3DProg/beziercurve.cpp \
    3DProg/billboard.cpp \
    3DProg/boundingbox.cpp \
    camera.cpp \
    VisSim/collision.cpp \
    coordread.cpp \
    3DProg/cube.cpp \
    debug.cpp \
    3DProg/disc.cpp \
    3DProg/door.cpp \
    3DProg/extentbox.cpp \
    3DProg/gamemode.cpp \
    3DProg/graph.cpp \
    VisSim/gravitasjonsball.cpp \
    3DProg/house.cpp \
    3DProg/interactiveobject.cpp \
    light.cpp \
    3DProg/lissajous.cpp \
    logger.cpp \
    mainwindow.cpp \
    matrix4x4.cpp \
    3DProg/npc.cpp \
    VisSim/objloader.cpp \
    VisSim/octahedronball.cpp \
    3DProg/plane.cpp \
    3DProg/player.cpp \
    renderwindow.cpp \
    shader.cpp \
    3DProg/skybox.cpp \
    stb_image/stb_image.cpp \
    3DProg/terrain.cpp \
    3DProg/tetrahedron.cpp \
    texture.cpp \
    VisSim/triangles.cpp \
    3DProg/trianglesurface.cpp \
    3DProg/trophy.cpp \
    vector3d.cpp \
    vector4d.cpp \
    vertex.cpp \
    3DProg/visualleastsquaresmethod.cpp \
    visualobject.cpp \
    VisSim/xyz.cpp

HEADERS += \
    Input.h \
    Toolbox.h \
    3DProg/beziercurve.h \
    3DProg/billboard.h \
    3DProg/boundingbox.h \
    camera.h \
    VisSim/collision.h \
    coordread.h \
    3DProg/cube.h \
    debug.h \
    3DProg/disc.h \
    3DProg/door.h \
    3DProg/extentbox.h \
    3DProg/gamemode.h \
    3DProg/graph.h \
    VisSim/gravitasjonsball.h \
    3DProg/house.h \
    3DProg/interactiveobject.h \
    light.h \
    3DProg/lissajous.h \
    logger.h \
    mainwindow.h \
    matrix4x4.h \
    3DProg/npc.h \
    VisSim/objloader.h \
    VisSim/octahedronball.h \
    3DProg/plane.h \
    3DProg/player.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    3DProg/skybox.h \
    stb_image/stb_image.h \
    3DProg/terrain.h \
    3DProg/tetrahedron.h \
    texture.h \
    VisSim/triangles.h \
    3DProg/trianglesurface.h \
    3DProg/trophy.h \
    vector3d.h \
    vector4d.h \
    vertex.h \
    3DProg/visualleastsquaresmethod.h \
    visualobject.h \
    VisSim/xyz.h

    # external libs
    glm/glm.hpp
    stb_image/stb_image.h
    gsl/gsl.h
    gsl/gsl.cpp

FORMS += \
    mainwindow.ui

DISTFILES += \
    billboardshader.frag \
    billboardshader.vert \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    skybox.frag \
    skybox.vert \
    textureshader.frag \
    textureshader.vert

#win32
#{
## Copy required DLL to output directory
#        CONFIG(debug, debug|release)
#        {
#            copydata.commands = copy /Y \"$$PWD\\Triangles.txt\" debug
#            copydata.target = debug/Triangles.txt

###            QMAKE_EXTRA_TARGETS += OpenAL32
###            PRE_TARGETDEPS += debug/OpenAL32.dll
###        } else:CONFIG(release, debug|release) {
###            OpenAL32.commands = copy /Y \"$$PWD\\AL\\OpenAL32.dll\" release
###            OpenAL32.target = release/OpenAL32.dll

###            QMAKE_EXTRA_TARGETS += OpenAL32
###            PRE_TARGETDEPS += release/OpenAL32.dll release/OpenAL32.dll
###        } else {
###            error(Unknown set of dependencies.)
#        }
#}
