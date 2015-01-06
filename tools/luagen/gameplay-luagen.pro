#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = gameplay-luagen
CONFIG   += console
CONFIG   -= app_bundle

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

DEPENDPATH += INCLUDEPATH

linux:!android: DEFINES += __linux__

linux:!android: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w

linux:!android: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps
linux:!android: LIBS += -lstdc++

linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/lib/linux/x86_64/libgameplay-deps.a
