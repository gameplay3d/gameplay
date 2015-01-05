#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT -= core gui

TARGET = sample-racer
TEMPLATE = app

SOURCES += src/RacerGame.cpp

HEADERS += src/RacerGame.h 

INCLUDEPATH += $$PWD/../../gameplay/src
INCLUDEPATH += $$PWD/../../external-deps/include
INCLUDEPATH += $$PWD/../../external-deps/include/bullet

linux:!android: INCLUDEPATH += /usr/include/gtk-2.0
linux:!android: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
linux:!android: INCLUDEPATH += /usr/include/atk-1.0
linux:!android: INCLUDEPATH += /usr/include/cairo
linux:!android: INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
linux:!android: INCLUDEPATH += /usr/include/pango-1.0
linux:!android: INCLUDEPATH += /usr/include/gio-unix-2.0
linux:!android: INCLUDEPATH += /usr/include/freetype2
linux:!android: INCLUDEPATH += /usr/include/glib-2.0
linux:!android: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
linux:!android: INCLUDEPATH += /usr/include/pixman-1
linux:!android: INCLUDEPATH += /usr/include/libpng12
linux:!android: INCLUDEPATH += /usr/include/harfbuzz

linux:!android: DEFINES += GP_USE_GAMEPAD
linux:!android: DEFINES += __linux__
linux:!android: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w

linux:!android: LIBS += -L$$PWD/../../gameplay/Debug/ -lgameplay
linux:!android: LIBS += -L$$PWD/../../external-deps/libs/Linux/x86_64/ -lgameplay-deps
linux:!android: LIBS += -lm
linux:!android: LIBS += -lGL
linux:!android: LIBS += -lrt
linux:!android: LIBS += -ldl
linux:!android: LIBS += -lX11
linux:!android: LIBS += -lpthread
linux:!android: LIBS += -lgtk-x11-2.0
linux:!android: LIBS += -lglib-2.0
linux:!android: LIBS += -lgobject-2.0

linux:!android: PRE_TARGETDEPS += $$PWD/../../gameplay/Debug/libgameplay.a
linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/libs/Linux/x86_64/libgameplay-deps.a

linux:!android: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
linux:!android: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
linux:!android: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))
linux:!android: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/game.dxt.config ../game.config$$escape_expand(\n\t))

