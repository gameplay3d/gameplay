QT -= core gui
TARGET = gameplay-luagen
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
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

INCLUDEPATH += $$PWD/../external-deps/include

win32 {
    DEFINES += _WINDOWS WIN32 _UNICODE UNICODE
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
    CONFIG(release, debug|release): LIBS += -L$$PWD/../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
    LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS_WARN_ON -= -w34189
    QMAKE_CXXFLAGS_WARN_ON -= -w4302
}

linux {
    DEFINES += __linux__
    QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w
    LIBS += -L$$PWD/../external-deps/lib/linux/x86_64/ -lgameplay-deps
    LIBS += -lstdc++
}

macx {
    QMAKE_CXXFLAGS += -x c++ -stdlib=libc++ -w -arch x86_64
    LIBS += -L$$PWD/../external-deps/lib/macos/x86_64/ -lgameplay-deps
}

