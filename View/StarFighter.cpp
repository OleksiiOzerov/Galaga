#include "StarFighter.hpp"
#include "GraphicsScene.hpp"

#include <QGraphicsScene>
#include <QState>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QKeyEventTransition>
#include <QDebug>
#include <QKeyEvent>
#include <QHistoryState>

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
        qDebug() << "eventTest";

        if (!QKeyEventTransition::eventTest(event))
        {
            event->ignore();
            return false;
        }
        return true;

//        if(event->type() == QEvent::KeyPress)
//        {
//            QKeyEvent *key = static_cast<QKeyEvent *>(event);

//            qDebug() << "key->key() " << key->key();
//            if(key->key() == Qt::Key_Right)
//            {
//                qDebug() << "true";
//                return true;
//            }
//            else
//                return false;
//        }
//        return false;
    }
    void onTransition(QEvent *) Q_DECL_OVERRIDE
    {
        qDebug() << "onTransition";
    }
};
//This transition trigger the bombs launch
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
        qDebug() << "KeyLaunchTransition eventTest";
        if (!QKeyEventTransition::eventTest(event))
        {
            return false;
            event->ignore();
        }
        return true;
    }
};


StarFighter::StarFighter() : PixmapItem(QString(":/Pictures/starfighter")),
    direction(StarFighter::None),
    movementAnimation(0)
{
    setFlags(QGraphicsItem::ItemIsFocusable);

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
    qDebug() << "StarFighter::fire";
    PixmapItem *missile = new PixmapItem(":/Pictures/bomb");
    missile->setZValue(1);
    missile->setX(x() + size().width() / 2 - missile->size().width() / 2);
    missile->setY(y());
    GraphicsScene *scene = static_cast<GraphicsScene *>(this->scene());
    scene->addItem(missile);

    QPropertyAnimation * missleAnimation = new QPropertyAnimation(missile, "pos");

    missleAnimation->setEndValue(QPointF(missile->x(), qreal(-10)));
    missleAnimation->setDuration(1000);
    missleAnimation->start();
}
