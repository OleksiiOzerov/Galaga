#include "Enemy.hpp"
#include "AnimationState.hpp"

#include <QPropertyAnimation>
#include <QStateMachine>
#include <QFinalState>
#include <QSequentialAnimationGroup>

static QAbstractAnimation *setupDestroyAnimation(Enemy *enemy)
{
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(enemy);
    for (int i = 1; i <= 4; ++i) {
        PixmapItem *step = new PixmapItem(QString("explosion/step%1").arg(i), enemy);
        step->setZValue(6);
        step->setOpacity(0);
        QPropertyAnimation *anim = new QPropertyAnimation(step, "opacity", group);
        anim->setDuration(100);
        anim->setEndValue(1);
    }
    return group;
}


Enemy::Enemy() : PixmapItem(QString("enemy"))
{
    setZValue(2);

    //We setup the state machine of the submarine
    QStateMachine *machine = new QStateMachine(this);

    //This state is when the boat is moving/rotating
    QState *moving = new QState(machine);

    //This is the initial state of the machine
    machine->setInitialState(moving);

    //End
    QFinalState *final = new QFinalState(machine);

    //This state play the destroyed animation
    AnimationState *destroyedState = new AnimationState(machine);
    destroyedState->setAnimation(setupDestroyAnimation(this));

    //Play a nice animation when the submarine is destroyed
    moving->addTransition(this, SIGNAL(enemyDestroyed()), destroyedState);

    //Transition to final state when the destroyed animation is finished
    destroyedState->addTransition(destroyedState, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the submarine is dead
    connect(machine,SIGNAL(finished()),this, SIGNAL(enemyExecutionFinished()));

    machine->start();
}

void Enemy::destroy()
{
    emit enemyDestroyed();
}

int Enemy::type() const
{
    return Type;
}
