#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T14:37:20
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Galaga
TEMPLATE = app


SOURCES += main.cpp \
    View/MainWindow.cpp \
    View/GraphicsScene.cpp \
    View/PixmapItem.cpp \
    View/StarFighter.cpp \
    View/GameStates.cpp \
    View/AnimationState.cpp \
    View/Rocket.cpp \
    View/Enemy.cpp

HEADERS  += \
    View/MainWindow.hpp \
    View/GraphicsScene.hpp \
    View/PixmapItem.hpp \
    View/StarFighter.hpp \
    View/GameStates.hpp \
    View/AnimationState.hpp \
    View/Rocket.hpp \
    View/Enemy.hpp

RESOURCES += \
    Galaga.qrc
