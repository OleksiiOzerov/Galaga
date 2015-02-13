
#include "View/MainWindow.hpp"

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow w;
    w.show();

    return app.exec();
}
