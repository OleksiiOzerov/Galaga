#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "PixmapItem.hpp"

class Rocket : public PixmapItem
{
    Q_OBJECT

public:
    Rocket();

    void launch();

signals:
    void rocketExploded();
    void rocketExecutionFinished();

private slots:
    void onAnimationLaunchValueChanged(const QVariant &);

private:
    void explode();
};

#endif // ROCKET_HPP
