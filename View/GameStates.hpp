#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <QState>

class GraphicsScene;

class PlayingGameState : public QState
{
    Q_OBJECT
public:
    explicit PlayingGameState(GraphicsScene * scene, QState *parent = 0);

protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE;

private slots:
    void allEnemiesDestroyed();

private:
    void startLevel();
    void startWave();

    int m_CurrentLevel;
    int m_CurrentWave;

    GraphicsScene *m_scene;
    QStateMachine *m_stateMachine;
};


#endif // ANIMATIONSTATE_H

