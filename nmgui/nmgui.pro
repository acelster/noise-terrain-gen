# Add more folders to ship with the application, here
#folder_01.source = qml/noisemodeler
#folder_01.target = qml
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creators code model
QML_IMPORT_PATH =

TARGET = nmgui
QT += core gui qml quick svg
CONFIG += static
QTPLUGIN += qsvg

QMAKE_CXXFLAGS += -std=c++11 \
    -Wall -Werror -Wextra \
    -pedantic-errors -Wwrite-strings \
    -Woverloaded-virtual -Wredundant-decls \
    -Wold-style-cast

# Qt doesn't like these warnings
#QMAKE_CXXFLAGS += -Weffc++ -Wuseless-cast -Wzero-as-null-pointer-constant -Wsign-conversion -Wctor-dtor-privacy -Wshadow

SOURCES += main.cpp \
    beziercurve.cpp \
    document.cpp \
    graphq.cpp \
    heightmap3dexplorer.cpp \
    heightmapfunction.cpp \
    heightmaptextureexplorer.cpp \
    inputlinkq.cpp \
    moduleinputq.cpp \
    moduleoutputq.cpp \
    moduleq.cpp \
    moduletypeq.cpp \
    outputlinkq.cpp \
    rendering/heightmap3drenderer.cpp \
    rendering/heightmapterrainmesh.cpp \
    rendering/heightmaptexturerenderer.cpp \
    rendering/transform3d.cpp \
    typemanagerq.cpp

HEADERS += \
    beziercurve.hpp \
    document.hpp \
    graphq.hpp \
    heightmap3dexplorer.hpp \
    heightmapfunction.hpp \
    heightmaptextureexplorer.hpp \
    inputlinkq.hpp \
    moduleinputq.hpp \
    moduleoutputq.hpp \
    moduleq.hpp \
    moduletypeq.hpp \
    outputlinkq.hpp \
    rendering/heightmap3drenderer.hpp \
    rendering/heightmapterrainmesh.hpp \
    rendering/heightmaptexturerenderer.hpp \
    rendering/transform3d.hpp \
    typemanagerq.hpp

# Installation path
# target.path =

INCLUDEPATH = . ..

win32{
    Debug {
        LIBS += -L../nmlib/debug
        PRE_TARGETDEPS += ../nmlib/debug/libnmlib.a
    }
    Release {
        LIBS += -L../nmlib/release
        PRE_TARGETDEPS += ../nmlib/release/libnmlib.a
    }
} else {
    LIBS += -L../nmlib
    PRE_TARGETDEPS += ../nmlib/libnmlib.a
}
LIBS += -lnmlib

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    nmgui.qrc
