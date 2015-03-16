#include "Rocket.hpp"
#include "GraphicsScene.hpp"
#include "AnimationState.hpp"
#include "Enemy.hpp"

#include <QPropertyAnimation>
#include <QStateMachine>
#include <QFinalState>
#include <QDebug>

Rocket::Rocket() : PixmapItem(QString("rocket"))
{
    setZValue(4);
}

void Rocket::launch()
{
    QPropertyAnimation *launchAnimation = new QPropertyAnimation(this, "pos");
    launchAnimation->setEndValue(QPointF(x(), 0 - 15));
    launchAnimation->setEasingCurve(QEasingCurve::InQuad);
    launchAnimation->setDuration(y()*2);

    connect(launchAnimation,SIGNAL(valueChanged(QVariant)),
            this, SLOT(onAnimationLaunchValueChanged(QVariant)));

    connect(this, SIGNAL(rocketExploded()),
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
    launched->addTransition(this, SIGNAL(rocketExploded()), final);

    //If the animation is finished, then we move to the final state
    launched->addTransition(launched, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the boat is dead
    connect(machine, SIGNAL(finished()),this, SIGNAL(rocketExecutionFinished()));

    machine->start();
}

void Rocket::onAnimationLaunchValueChanged(const QVariant &)
{
    foreach (QGraphicsItem *item , collidingItems(Qt::IntersectsItemBoundingRect))
    {
        if (Enemy *enemy = qgraphicsitem_cast<Enemy*>(item))
        {
            enemy->destroy();
            explode();
        }
    }
}

void Rocket::explode()
{
    emit rocketExploded();
}
