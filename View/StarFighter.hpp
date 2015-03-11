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

    StarFighter();

    void fly();

    void stop();

    void moveLeft();

    void moveRight();


    void fire();

private:

    Movement direction;
    QVariantAnimation *movementAnimation;
    QStateMachine *machine;
};

#endif // STARFIGHTER_HPP
