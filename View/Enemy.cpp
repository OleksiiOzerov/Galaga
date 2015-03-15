#include "Enemy.hpp"

Enemy::Enemy() : PixmapItem(QString(":/Pictures/enemy"))
{
    setZValue(2);
}

int Enemy::type() const
{
    return Type;
}
