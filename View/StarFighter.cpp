#include "StarFighter.hpp"
#include "GraphicsScene.hpp"
#include "AnimationState.hpp"
#include "Rocket.hpp"

#include <QGraphicsScene>
#include <QState>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QKeyEventTransition>
#include <QDebug>
#include <QKeyEvent>
#include <QHistoryState>
#include <QFinalState>
#include <QSequentialAnimationGroup>

class StopState : public QState
{
public:
    explicit StopState(StarFighter *starFighter, QState *parent = 0) : QState(parent),
        m_StarFighter(starFighter)
    {
    }
protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "Stop";
        m_StarFighter->stop();
    }
private:
    StarFighter * m_StarFighter;
};


//This state is describing when the boat is moving right
class MoveStateRight : public QState
{
public:
    explicit MoveStateRight(StarFighter *starFighter, QState *parent = 0) : QState(parent),
        m_StarFighter(starFighter)
    {
    }
protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "MoveStateRight";
        m_StarFighter->moveRight();
    }
private:
    StarFighter * m_StarFighter;
};

 //This state is describing when the boat is moving left
class MoveStateLeft : public QState
{
public:
    explicit MoveStateLeft(StarFighter *starFighter, QState *parent = 0) : QState(parent),
        m_StarFighter(starFighter)
    {
    }
protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "MoveStateLeft";
        m_StarFighter->moveLeft();
    }
private:
    StarFighter * m_StarFighter;
};

class LaunchState : public QState
{
public:
    explicit LaunchState(StarFighter *starFighter, QState *parent = 0) : QState(parent),
        m_StarFighter(starFighter)
    {
    }
protected:
    void onEntry(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "LaunchState";
        m_StarFighter->fire();
    }
private:
    StarFighter * m_StarFighter;
};

class KeyTransition : public QKeyEventTransition
{
public:
    KeyTransition(StarFighter *b, QEvent::Type t, int k)
    : QKeyEventTransition(b, t, k)
    {
    }
protected:
    bool eventTest(QEvent *event) Q_DECL_OVERRIDE
    {
        if (!QKeyEventTransition::eventTest(event))
        {
            event->ignore();
            return false;
        }
        return true;
    }
    void onTransition(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "onTransition";
    }
};

 class KeyLaunchTransition : public QKeyEventTransition
{
public:
    KeyLaunchTransition(StarFighter *starFighter, QEvent::Type type, int key)
    : QKeyEventTransition(starFighter, type, key)
    {
    }
protected:
    virtual bool eventTest(QEvent *event) Q_DECL_OVERRIDE
    {
        if (!QKeyEventTransition::eventTest(event))
        {
            return false;
            event->ignore();
        }
        return true;
    }
};

static QAbstractAnimation *setupDestroyAnimation(StarFighter *starFighter)
{
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(starFighter);
    for (int i = 1; i <= 4; ++i) {
         PixmapItem *step = new PixmapItem(QString("explosion/step%1").arg(i), starFighter);
         step->setZValue(6);
         step->setOpacity(0);
         QPropertyAnimation *anim = new QPropertyAnimation(step, "opacity", group);
         anim->setDuration(100);
         anim->setEndValue(1);
    }
    return group;
}

StarFighter::StarFighter() : PixmapItem(QString("starfighter")),
    direction(StarFighter::None),
    movementAnimation(0)
{
    setFlags(QGraphicsItem::ItemIsFocusable);
    setZValue(2);

    movementAnimation = new QPropertyAnimation(this, "pos");

    machine = new QStateMachine(this);
    QState *moving = new QState(machine);
    StopState *stopState = new StopState(this, moving);
    machine->setInitialState(moving);
    MoveStateRight *moveStateRight = new MoveStateRight(this, moving);
    MoveStateLeft *moveStateLeft = new MoveStateLeft(this, moving);
    LaunchState *fireState = new LaunchState(this, machine);
    moving->setInitialState(stopState);

    KeyTransition *startMoveRight = new KeyTransition(this, QEvent::KeyPress, Qt::Key_Right);
    startMoveRight->setTargetState(moveStateRight);
    stopState->addTransition(startMoveRight);

    KeyTransition *stopFromRight = new KeyTransition(this, QEvent::KeyPress, Qt::Key_Left);
    stopFromRight->setTargetState(stopState);
    moveStateRight->addTransition(stopFromRight);

    KeyTransition *startMoveLeft = new KeyTransition(this, QEvent::KeyPress, Qt::Key_Left);
    startMoveLeft->setTargetState(moveStateLeft);
    stopState->addTransition(startMoveLeft);

    QKeyEventTransition *stopFromLeft = new KeyTransition(this, QEvent::KeyPress, Qt::Key_Right);
    stopFromLeft->setTargetState(stopState);
    moveStateLeft->addTransition(stopFromLeft);

    moveStateLeft->addTransition(movementAnimation, SIGNAL(finished()), stopState);
    moveStateRight->addTransition(movementAnimation, SIGNAL(finished()), stopState);

    KeyLaunchTransition *fireTransitionFromStopState = new KeyLaunchTransition(this, QEvent::KeyPress, Qt::Key_Space);
    fireTransitionFromStopState->setTargetState(fireState);
    KeyLaunchTransition *fireTransitionFromMoveLeftState = new KeyLaunchTransition(this, QEvent::KeyPress, Qt::Key_Space);
    fireTransitionFromMoveLeftState->setTargetState(fireState);
    KeyLaunchTransition *fireTransitionFromMoveRightState = new KeyLaunchTransition(this, QEvent::KeyPress, Qt::Key_Space);
    fireTransitionFromMoveRightState->setTargetState(fireState);

    stopState->addTransition(fireTransitionFromStopState);
    moveStateLeft->addTransition(fireTransitionFromMoveLeftState);
    moveStateRight->addTransition(fireTransitionFromMoveRightState);

    QHistoryState *historyState = new QHistoryState(moving);
    fireState->addTransition(historyState);

    //End
    QFinalState *final = new QFinalState(machine);

    //This state play the destroyed animation
    AnimationState *destroyedState = new AnimationState(machine);
    destroyedState->setAnimation(setupDestroyAnimation(this));

    //Play a nice animation when the submarine is destroyed
    moving->addTransition(this, SIGNAL(starFighterDestroyed()), destroyedState);

    //Transition to final state when the destroyed animation is finished
    destroyedState->addTransition(destroyedState, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the submarine is dead
    connect(machine,SIGNAL(finished()),this, SIGNAL(starFighterExecutionFinished()));
}

void StarFighter::fly()
{
    machine->start();
}

void StarFighter::stop()
{
    movementAnimation->stop();
}

void StarFighter::moveLeft()
{
    if (movementAnimation->state() != QAbstractAnimation::Running)
    {
        movementAnimation->setEndValue(QPointF(0,y()));
        movementAnimation->setDuration(x() * 5);
        movementAnimation->start();
    }
}

void StarFighter::moveRight()
{
    if (movementAnimation->state() != QAbstractAnimation::Running)
    {
        movementAnimation->setEndValue(QPointF(scene()->width()-size().width(),y()));
        movementAnimation->setDuration((scene()->width()-size().width()-x()) * 5);
        movementAnimation->start();
    }
}

void StarFighter::fire()
{
    //qDebug() << "StarFighter::fire";

    Rocket *rocket = new Rocket();

    rocket->setX(x() + size().width() / 2 - rocket->size().width() / 2);
    rocket->setY(y() - rocket->size().height());
    GraphicsScene *scene = static_cast<GraphicsScene *>(this->scene());
    scene->addItem(rocket);
    rocket->launch();
}

void StarFighter::destroy()
{
    stop();
    emit starFighterDestroyed();
}

int StarFighter::type() const
{
    return Type;
}
