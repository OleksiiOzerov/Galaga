#ifndef GRAPHICSVIEW_HP
#define GRAPHICSVIEW_HP

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QSequentialAnimationGroup;
class QParallelAnimationGroup;
//class QGraphicsTextItem;
QT_END_NAMESPACE

class StarFighter;
class PixmapItem;
class Rocket;
class Enemy;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(int x, int y, int width, int height);

    void setupGameStateMachine();

    void addItem(Rocket *rocket);
    void addItem(Enemy *enemy);
    void addItem(QGraphicsItem *item);

private slots:
    void onRocketExecutionFinished();
    void onEnemyExecutionFinished();

protected:
    //void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:

    friend class PlayingGameState;

    void setGameBackground(int sceneWidth, int sceneHeight);

    void createGameLogoAnimation(int sceneWidth, int sceneHeight);

    void createStarfighter();

    static const int screenHeightCoefficient = 13;

    StarFighter * m_StarFighter;
    QGraphicsTextItem *m_textInformation;
    QSequentialAnimationGroup * gameLogoAnimation;
    QParallelAnimationGroup * gameLogoFadingAnimation;
};

#endif // GRAPHICSVIEW_HP
