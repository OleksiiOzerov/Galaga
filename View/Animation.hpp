#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <QPropertyAnimation>
#include <QPainterPath>

class Animation : public QPropertyAnimation
{
public:
    Animation(QObject *target, const QByteArray &prop);

    void addPause(int milliSeconds);

    void setDuration(int milliSeconds);

    void setScreenResolution(int screenWidth, int screenHeight);

    void updateCurrentTime(int currentTime) Q_DECL_OVERRIDE;
private:

    int m_Pause;

    int m_ScreenWidth;

    int m_ScreenHeight;

    QPainterPath m_Path;
};

#endif // ANIMATION_HPP
