#ifndef GRAPHICSVIEW_HP
#define GRAPHICSVIEW_HP

#include <QGraphicsScene>

class QSequentialAnimationGroup;
class QParallelAnimationGroup;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(int x, int y, int width, int height);

private slots:
    void GameStarted();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void StartGame();

    QSequentialAnimationGroup * lettersGroupMoving;
    QParallelAnimationGroup * lettersGroupFading;
};

#endif // GRAPHICSVIEW_HP
