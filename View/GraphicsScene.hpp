#ifndef GRAPHICSVIEW_HP
#define GRAPHICSVIEW_HP

#include "PixmapItem.hpp"

#include <QGraphicsScene>
#include <QLinkedList>

class QSequentialAnimationGroup;
class QParallelAnimationGroup;

class QTimer;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(int x, int y, int width, int height);

private slots:
    void gameStarted();
    void updateMissiles();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void startGame();

    void moveLeft();
    void moveRight();

    void fire();

    bool m_GameStared;

    PixmapItem * m_StarFighter;

    QTimer * m_Timer;

    QLinkedList<PixmapItem> m_MissilesCollection;

    QSequentialAnimationGroup * lettersGroupMoving;
    QParallelAnimationGroup * lettersGroupFading;
};

#endif // GRAPHICSVIEW_HP
