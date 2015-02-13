#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QGraphicsView;
class GraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    GraphicsScene *m_Scene;
    QGraphicsView *m_View;
};

#endif // MAINWINDOW_HPP
