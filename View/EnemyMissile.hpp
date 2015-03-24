#ifndef ENEMYWEAPON_HPP
#define ENEMYWEAPON_HPP

#include "PixmapItem.hpp"

class EnemyMissile : public PixmapItem
{
    Q_OBJECT

public:
    EnemyMissile();

    void launch();

signals:
    void missileExploded();
    void missileExecutionFinished();

private slots:
    void onAnimationLaunchValueChanged(const QVariant &);

private:
    void explode();
};


#endif // ENEMYWEAPON_HPP
