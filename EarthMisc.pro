include(functions.pri)

TEMPLATE = app
CONFIG += console no_batch
QT += core widgets opengl

win32-msvc* {
    DEFINES -= UNICODE
}

LIB_PATH = $$PWD/sdk/libs
message($$LIB_PATH)

LIBS += \
    -L$$LIB_PATH

message($$libTarget(OpenThreads))

LIBS += \
    -l$$libTarget(OpenThreads) \
    -l$$libTarget(osgFX) \
    -l$$libTarget(osgPresentation) \
    -l$$libTarget(osgTerrain) \
    -l$$libTarget(osgViewer) \
    -l$$libTarget(osg) \
    -l$$libTarget(osgGA) \
    -l$$libTarget(osgText) \
    -l$$libTarget(osgVolume) \
    -l$$libTarget(osgAnimation) \
    -l$$libTarget(osgManipulator) \
    -l$$libTarget(osgShadow) \
    -l$$libTarget(osgUI) \
    -l$$libTarget(osgWidget) \
    -l$$libTarget(osgDB) \
    -l$$libTarget(osgParticle) \
    -l$$libTarget(osgSim) \
    -l$$libTarget(osgUtil) \
    -l$$libTarget(osgPPU) \
    -l$$libTarget(osgEphemeris) \
    -l$$libTarget(osgOcean) \
    -l$$libTarget(osgEarth) \
    -l$$libTarget(osgEarthQt) \
    -l$$libTarget(osgEarthUtil) \
    -l$$libTarget(osgEarthAnnotation) \
    -l$$libTarget(osgEarthFeatures) \
    -l$$libTarget(osgEarthSymbology)

INCLUDEPATH += \
    $$PWD/sdk/include/osg \
    sdk/include/osgearth


SOURCES += \
    $$PWD/src/EarthMisc.cpp \
    $$PWD/src/OverviewMap.cpp \
    $$PWD/src/ScaleBar.cpp \
    $$PWD/src/Compass.cpp \
    $$PWD/src/StatsHandler.cpp \

