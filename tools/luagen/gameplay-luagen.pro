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

INCLUDEPATH += $$PWD/../../external-deps/lua/include
INCLUDEPATH += $$PWD/../../external-deps/tinyxml2/include

DEPENDPATH += INCLUDEPATH

linux:!android: DEFINES += __linux__

linux:!android: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w

linux:!android: LIBS += -L$$PWD/../../external-deps/lua/lib/linux/x64/ -llua
linux:!android: LIBS += -L$$PWD/../../external-deps/tinyxml2/lib/linux/x64/ -ltinyxml2
linux:!android: LIBS += -lstdc++

linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/lua/lib/linux/x64/liblua.a
linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/tinyxml2/lib/linux/x64/libtinyxml2.a

