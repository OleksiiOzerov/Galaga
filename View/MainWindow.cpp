
#include "MainWindow.hpp"
#include "GraphicsScene.hpp"

#include <QGraphicsView>
#include <QDebug>

#ifndef QT_NO_OPENGL
# include <QtOpenGL/QtOpenGL>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowState(Qt::WindowFullScreen);

    qDebug() << "height " << geometry().height();
    qDebug() << "width " << geometry().width();

    m_Scene = new GraphicsScene(0, 0, geometry().width() - 2, geometry().height() - 2);

    m_View = new QGraphicsView(m_Scene, this);
    m_View->setAlignment(Qt::AlignCenter);

    //scene->setupScene(newAction, quitAction);

//    #ifndef QT_NO_OPENGL
//    m_View->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//    #endif

    //resize(1005, 605);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);

    setCentralWidget(m_View);
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "events";
    switch (event->key())
    {
    case Qt::Key_Escape:
        qDebug() << "Key_Escape";
        qApp->quit();
        break;
    }
}
