#ifndef GRAPHICSVIEW_HP
#define GRAPHICSVIEW_HP

#include "LevelDescription.hpp"

#include <QGraphicsScene>
#include <QSet>

QT_BEGIN_NAMESPACE
class QSequentialAnimationGroup;
class QParallelAnimationGroup;
//class QGraphicsTextItem;
QT_END_NAMESPACE

class StarFighter;
class PixmapItem;
class Rocket;
class Enemy;
class EnemyMissile;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(int x, int y, int width, int height);

    void setupGameStateMachine();

    void addItem(Rocket *rocket);
    void addItem(Enemy *enemy);
    void addItem(EnemyMissile *enemyMissile);
    void addItem(QGraphicsItem *item);

signals:
    void gameOver();

private slots:
    void onRocketExecutionFinished();
    void onEnemyExecutionFinished();
    void onMissileExecutionFinished();
    void onStarFighterExecutionFinished();

protected:
    //void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    friend class PlayingGameState;

    void setGameBackground(int sceneWidth, int sceneHeight);
    void createGameLogoAnimation(int sceneWidth, int sceneHeight);
    void createStarfighter();
    void setupGameData();

    static const int screenHeightCoefficient = 13;

    QList<LevelDescription> m_GameDescription;
    QSet<Enemy *> m_Enemies;
    StarFighter * m_StarFighter;
    QGraphicsTextItem *m_textInformation;
    QSequentialAnimationGroup * gameLogoAnimation;
    QParallelAnimationGroup * gameLogoFadingAnimation;
};

#endif // GRAPHICSVIEW_HP
