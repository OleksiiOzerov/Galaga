
#include "MainWindow.hpp"
#include "GraphicsScene.hpp"

#include <QGraphicsView>

#ifndef QT_NO_OPENGL
# include <QtOpenGL/QtOpenGL>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_Scene = new GraphicsScene(0, 0, 1280, 720);

    m_View = new QGraphicsView(m_Scene, this);
    m_View->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //scene->setupScene(newAction, quitAction);

//    #ifndef QT_NO_OPENGL
//    m_View->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//    #endif

    setCentralWidget(m_View);
}

MainWindow::~MainWindow()
{

}
