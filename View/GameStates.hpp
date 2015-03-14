#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <QState>

class GraphicsScene;

class PlayingGameState : public QState
{
public:
    explicit PlayingGameState(GraphicsScene * scene, QState *parent = 0);

protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE;

private:

    GraphicsScene *m_scene;
    QStateMachine *m_stateMachine;
};


#endif // ANIMATIONSTATE_H

