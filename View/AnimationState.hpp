#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include <QState>
#include <QAbstractAnimation>


class AnimationState : public QState
{
    Q_OBJECT
public:
    explicit AnimationState(QState *parent = 0);

    void setAnimation(QAbstractAnimation* animation);
    QAbstractAnimation* animation() const;

signals:
    void animationFinished();

protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE;
    void onExit(QEvent *) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(AnimationState)

    QAbstractAnimation *m_animation;
};


class GameStartState : public QState
{
    Q_OBJECT
public:
    explicit GameStartState(QState *parent = 0) : QState(parent) {}

signals:
    void startGame();

protected:
    void onEntry(QEvent *)
    {
        emit startGame();
    }


private:
    Q_DISABLE_COPY(GameStartState)

};


#endif // ANIMATIONSTATE_H

