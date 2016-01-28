#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT -= core gui
TARGET = sample-racer
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

SOURCES += src/RacerGame.cpp

HEADERS += src/RacerGame.h 

INCLUDEPATH += $$PWD/../../gameplay/src
INCLUDEPATH += $$PWD/../../external-deps/include
DEFINES += GP_USE_GAMEPAD

linux: DEFINES += __linux__
linux: QMAKE_CXXFLAGS += -lstdc++ -pthread -w
linux: INCLUDEPATH += /usr/include/gtk-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
linux: INCLUDEPATH += /usr/include/atk-1.0
linux: INCLUDEPATH += /usr/include/cairo
linux: INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
linux: INCLUDEPATH += /usr/include/pango-1.0
linux: INCLUDEPATH += /usr/include/gio-unix-2.0
linux: INCLUDEPATH += /usr/include/freetype2
linux: INCLUDEPATH += /usr/include/glib-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
linux: INCLUDEPATH += /usr/include/pixman-1
linux: INCLUDEPATH += /usr/include/libpng12
linux: INCLUDEPATH += /usr/include/harfbuzz
linux: LIBS += -L$$PWD/../../gameplay/Debug/ -lgameplay
linux: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps
linux: LIBS += -lm -lGL -lrt -ldl -lX11 -lpthread -lgtk-x11-2.0 -lglib-2.0 -lgobject-2.0
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/game.dxt.config game.config$$escape_expand(\n\t))

macx: QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w -arch x86_64
macx: LIBS += -L$$PWD/../../gameplay/Debug/ -lgameplay
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lgameplay-deps
macx: LIBS += -F/System/Library/Frameworks -framework GameKit
macx: LIBS += -F/System/Library/Frameworks -framework IOKit
macx: LIBS += -F/System/Library/Frameworks -framework QuartzCore
macx: LIBS += -F/System/Library/Frameworks -framework OpenAL
macx: LIBS += -F/System/Library/Frameworks -framework OpenGL
macx: LIBS += -F/System/Library/Frameworks -framework Cocoa
macx: LIBS += -F/System/Library/Frameworks -framework Foundation
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))
macx 
{
    icon.files = icon.png
    icon.path = Contents/Resources
    QMAKE_BUNDLE_DATA += icon

    gameconfig.files = game.config
    gameconfig.path = Contents/Resources
    QMAKE_BUNDLE_DATA += gameconfig

    res.files = res
    res.path = Contents/Resources
    QMAKE_BUNDLE_DATA += res
}

win32: DEFINES += WIN32 _WINDOWS _UNICODE UNICODE
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gameplay/Debug/debug/ -lgameplay
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../gameplay/Release/release/ -lgameplay
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
win32: LIBS += -lOpenGL32 -lGLU32 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
win32: LIBS += -L$$(DXSDK_DIR)Lib\x64 -lXInput
win32: INCLUDEPATH += $$(DXSDK_DIR)Include
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_CXXFLAGS_WARN_ON -= -w4302
win32: QMAKE_POST_LINK += $$quote(xcopy ..\..\..\gameplay\res\shaders res\shaders\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(xcopy ..\..\..\gameplay\res\ui res\ui\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(copy ..\..\..\gameplay\res\logo_powered_white.png res$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(copy /Y ..\game.dxt.config game.config$$escape_expand(\n\t))
