#include "AnimationState.hpp"
#include <QAbstractAnimation>

AnimationState::AnimationState(QState *parent) :
    QState(parent), m_animation(0)
{
}

void AnimationState::setAnimation(QAbstractAnimation* animation)
{
    if (animation == m_animation)
        return;

    //Disconnect from the previous animation if exist
    if(m_animation)
        disconnect(m_animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));

    m_animation = animation;

    if (m_animation)
        //connect the new animation
        connect(m_animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));

}

QAbstractAnimation* AnimationState::animation() const
{
    return m_animation;
}

void AnimationState::onEntry(QEvent *)
{
    if (m_animation)
        m_animation->start();
}

void AnimationState::onExit(QEvent *)
{
    if (m_animation)
        m_animation->stop();
}

bool AnimationState::event(QEvent *e)
{
    return QState::event(e);
}
