#ifndef STARFIGHTER_HPP
#define STARFIGHTER_HPP

#include "PixmapItem.hpp"

#include <QKeyEventTransition>
#include <QDebug>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
class QVariantAnimation;
class QAbstractAnimation;
class QStateMachine;

QT_END_NAMESPACE

class StarFighter : public PixmapItem
{
    Q_OBJECT

public:
    enum Movement
    {
       None = 0,
       Left,
       Right
    };

    enum { Type = UserType + 2};

    StarFighter();

    void fly();
    void stop();
    void destroy();
    void fire();

    void moveLeft();
    void moveRight();

    virtual int type() const Q_DECL_OVERRIDE;

signals:
    void starFighterDestroyed();
    void starFighterExecutionFinished();

private:

    Movement direction;
    QVariantAnimation *movementAnimation;
    QStateMachine *machine;
};

#endif // STARFIGHTER_HPP
