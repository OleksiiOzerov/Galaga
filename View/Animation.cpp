#include "Animation.hpp"

#include <QDebug>

Animation::Animation(QObject *target, const QByteArray &prop)
    : QPropertyAnimation(target, prop),
      m_Pause(0),
      m_ScreenWidth(0),
      m_ScreenHeight(0)
{
}

void Animation::addPause(int milliSeconds)
{
    m_Pause = milliSeconds;

    setDuration(duration() + milliSeconds);
}

void Animation::setDuration(int milliSeconds)
{
    QPropertyAnimation::setDuration(m_Pause + milliSeconds);
}

void Animation::setScreenResolution(int screenWidth, int screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void Animation::updateCurrentTime(int currentTime)
{
    if (m_Path.isEmpty())
    {
        QPointF to = endValue().toPointF();
        QPointF from = currentValue().toPointF();

        m_Path.moveTo(from);
        m_Path.lineTo(from + QPointF(0, 150));

        QPointF ctrlPoint1(m_ScreenWidth, m_ScreenHeight / 2);

        QPointF ctrlPoint2(from.x(), m_ScreenHeight);

        m_Path.cubicTo(ctrlPoint1, ctrlPoint2, to);
    }

    if (currentTime > m_Pause)
    {
        currentTime -= m_Pause;

        int animationDuration = duration() - m_Pause;

        const qreal progress = ((animationDuration == 0) ? 1 : ((((currentTime - 1) % animationDuration) + 1) / qreal(animationDuration)));

        qreal easedProgress = easingCurve().valueForProgress(progress);

        if (easedProgress > 1.0)
        {
            easedProgress -= 1.0;
        }
        else if (easedProgress < 0)
        {
            easedProgress += 1.0;
        }

        QPointF pt = m_Path.pointAtPercent(easedProgress);

        updateCurrentValue(pt);
        emit valueChanged(pt);
    }
}
