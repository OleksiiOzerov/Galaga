#include "Animation.hpp"

#include <QDebug>

Animation::Animation(QObject *target, const QByteArray &prop)
    : QPropertyAnimation(target, prop),
      m_Pause(0)
{
}
void Animation::addPause(int milliSeconds)
{
    m_Pause = milliSeconds;
}

void Animation::updateCurrentTime(int currentTime)
{
    if (m_Path.isEmpty())
    {
        QPointF to = endValue().toPointF();
        QPointF from = currentValue().toPointF();

        m_Path.moveTo(from);
        m_Path.lineTo(from + QPointF(0, 200));

        QPointF c1 = from + QPointF(0, 1000);

        QPointF c2 = from + QPointF(from.x() / 2, 0);

        m_Path.cubicTo(c2, c1, to);
    }

    if (currentTime > m_Pause)
    {
        currentTime -= m_Pause;

        int dura = duration() - m_Pause;

        const qreal progress = ((dura == 0) ? 1 : ((((currentTime - 1) % dura) + 1) / qreal(dura)));

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
