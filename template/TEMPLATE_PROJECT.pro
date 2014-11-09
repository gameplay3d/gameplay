#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT -= core gui

TARGET = TEMPLATE_PROJECT
TEMPLATE = app

SOURCES += src/TemplateGame.cpp

HEADERS += src/TemplateGame.h

INCLUDEPATH += GAMEPLAY_PATH/gameplay/src
INCLUDEPATH += GAMEPLAY_PATH/external-deps/bullet/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/lua/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/png/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/zlib/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/ogg/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/vorbis/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/openal/include
INCLUDEPATH += GAMEPLAY_PATH/external-deps/glew/include
INCLUDEPATH += /usr/include/gtk-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
INCLUDEPATH += /usr/include/atk-1.0
INCLUDEPATH += /usr/include/cairo
INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
INCLUDEPATH += /usr/include/pango-1.0
INCLUDEPATH += /usr/include/gio-unix-2.0
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/pixman-1
INCLUDEPATH += /usr/include/libpng12
INCLUDEPATH += /usr/include/harfbuzz
DEPENDPATH += INCLUDEPATH

DEFINES += GP_USE_GAMEPAD

unix:!macx: DEFINES += __linux__
unix:!macx: QMAKE_CXXFLAGS += -fPIC -fmessage-length=0 -std=c++11 -lstdc++ -pthread -w

unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/gameplay/Debug/ -lgameplay
unix:!macx: LIBS += -lm
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/lua/lib/linux/x64/ -llua
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/png/lib/linux/x64/ -lpng
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/zlib/lib/linux/x64/ -lz
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/vorbis/lib/linux/x64/ -lvorbis
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/ogg/lib/linux/x64/ -logg
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/ -lBulletDynamics
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/ -lBulletCollision
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/ -lLinearMath
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/openal/lib/linux/x64/ -lopenal
unix:!macx: LIBS += -L$$PWD/GAMEPLAY_PATH/external-deps/glew/lib/linux/x64/ -lGLEW
unix:!macx: LIBS += -lGL
unix:!macx: LIBS += -lrt
unix:!macx: LIBS += -ldl
unix:!macx: LIBS += -lX11
unix:!macx: LIBS += -lpthread
unix:!macx: LIBS += -lgtk-x11-2.0
unix:!macx: LIBS += -lglib-2.0
unix:!macx: LIBS += -lgobject-2.0

unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/gameplay/Debug/libgameplay.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/lua/lib/linux/x64/liblua.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/libLinearMath.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/libBulletCollision.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/bullet/lib/linux/x64/libBulletDynamics.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/png/lib/linux/x64/libpng.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/zlib/lib/linux/x64/libz.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/ogg/lib/linux/x64/libogg.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/vorbis/lib/linux/x64/libvorbis.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/openal/lib/linux/x64/libopenal.a
unix:!macx: PRE_TARGETDEPS += $$PWD/GAMEPLAY_PATH/external-deps/glew/lib/linux/x64/libGLEW.a

unix:!macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/GAMEPLAY_PATH/gameplay/res/shaders ../res$$escape_expand(\n\t))
unix:!macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/GAMEPLAY_PATH/gameplay/res/ui ../res$$escape_expand(\n\t))
unix:!macx: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/GAMEPLAY_PATH/gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))

unix {
    target.path = /usr/lib
    INSTALLS += target
}
