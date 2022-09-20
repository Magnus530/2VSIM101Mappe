QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    beziercurve.cpp \
    billboard.cpp \
    boundingbox.cpp \
    camera.cpp \
    collision.cpp \
    cube.cpp \
    debug.cpp \
    disc.cpp \
    door.cpp \
    extentbox.cpp \
    gamemode.cpp \
    graph.cpp \
    gravitasjonsball.cpp \
    house.cpp \
    interactiveobject.cpp \
    light.cpp \
    lissajous.cpp \
    logger.cpp \
    mainwindow.cpp \
    matrix4x4.cpp \
    npc.cpp \
    objloader.cpp \
    octahedronball.cpp \
    plane.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    skybox.cpp \
    stb_image/stb_image.cpp \
    terrain.cpp \
    tetrahedron.cpp \
    texture.cpp \
    triangles.cpp \
    trianglesurface.cpp \
    trophy.cpp \
    vector3d.cpp \
    vector4d.cpp \
    vertex.cpp \
    visualleastsquaresmethod.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    Input.h \
    Toolbox.h \
    beziercurve.h \
    billboard.h \
    boundingbox.h \
    camera.h \
    collision.h \
    cube.h \
    debug.h \
    disc.h \
    door.h \
    extentbox.h \
    gamemode.h \
    graph.h \
    gravitasjonsball.h \
    house.h \
    interactiveobject.h \
    light.h \
    lissajous.h \
    logger.h \
    mainwindow.h \
    matrix4x4.h \
    npc.h \
    objloader.h \
    octahedronball.h \
    plane.h \
    player.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    skybox.h \
    stb_image/stb_image.h \
    terrain.h \
    tetrahedron.h \
    texture.h \
    triangles.h \
    trianglesurface.h \
    trophy.h \
    vector3d.h \
    vector4d.h \
    vertex.h \
    visualleastsquaresmethod.h \
    visualobject.h \
    xyz.h

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
