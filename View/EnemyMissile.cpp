#include "EnemyMissile.hpp"
#include "StarFighter.hpp"
#include "GraphicsScene.hpp"
#include "AnimationState.hpp"

#include <QPropertyAnimation>
#include <QStateMachine>
#include <QFinalState>
#include <QDebug>

EnemyMissile::EnemyMissile() : PixmapItem(QString("rocket"))
{
        setZValue(4);
}

void EnemyMissile::launch()
{
    QPropertyAnimation *launchAnimation = new QPropertyAnimation(this, "pos");
    launchAnimation->setEndValue(QPointF(x(), scene()->height()));
    launchAnimation->setEasingCurve(QEasingCurve::InQuad);
    launchAnimation->setDuration((scene()->height() - y()) * 2);

    connect(launchAnimation, SIGNAL(valueChanged(QVariant)),
            this, SLOT(onAnimationLaunchValueChanged(QVariant)));

    connect(this, SIGNAL(missileExploded()),
            launchAnimation, SLOT(stop()));

    //We setup the state machine of the torpedo
    QStateMachine *machine = new QStateMachine(this);

    //This state is when the launch animation is playing
    AnimationState *launched = new AnimationState(machine);
    launched->setAnimation(launchAnimation);

    //End
    QFinalState *final = new QFinalState(machine);

    machine->setInitialState(launched);

    //### Add a nice animation when the torpedo is destroyed
    launched->addTransition(this, SIGNAL(missileExploded()), final);

    //If the animation is finished, then we move to the final state
    launched->addTransition(launched, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the boat is dead
    connect(machine, SIGNAL(finished()),this, SIGNAL(missileExecutionFinished()));

    machine->start();
}

void EnemyMissile::onAnimationLaunchValueChanged(const QVariant &)
{
    foreach (QGraphicsItem *item , collidingItems(Qt::IntersectsItemBoundingRect))
    {
        if (StarFighter *starFighter = qgraphicsitem_cast<StarFighter*>(item))
        {
            starFighter->destroy();
            explode();
        }
    }
}

void EnemyMissile::explode()
{
    emit missileExploded();
}
