#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT       -= core gui
TARGET = gameplay-luagen
CONFIG   += console
CONFIG   -= app_bundle
CONFIG(debug, debug|release): DEFINES += _DEBUG

TEMPLATE = app

SOURCES += src/main.cpp \
    src/ClassBinding.cpp \
    src/DebugNew.cpp \
    src/FunctionBinding.cpp \
    src/Generator.cpp


HEADERS += src/Base.h \
    src/ClassBinding.h \
    src/DebugNew.h \
    src/EnumBinding.h \
    src/FunctionBinding.h \
    src/Generator.h \
    src/TypedefBinding.h

INCLUDEPATH += $$PWD/../../external-deps/include

linux: DEFINES += __linux__
linux: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w
linux: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps
linux: LIBS += -lstdc++

macx: QMAKE_CXXFLAGS += -x c++ -stdlib=libc++ -w -arch x86_64
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lgameplay-deps

win32: DEFINES += WIN32 _WINDOWS _UNICODE UNICODE
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
win32: LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_CXXFLAGS_WARN_ON -= -w4302
