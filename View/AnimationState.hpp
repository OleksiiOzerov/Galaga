#ifndef ANIMATIONSTATE_HPP
#define ANIMATIONSTATE_HPP

#include <QState>

QT_BEGIN_NAMESPACE
class QAbstractAnimation;
QT_END_NAMESPACE

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
#endif // ANIMATIONSTATE_HPP
