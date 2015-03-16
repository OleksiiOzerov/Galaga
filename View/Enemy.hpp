#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "PixmapItem.hpp"

class Enemy : public PixmapItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 1 };

    Enemy();

    void destroy();

    virtual int type() const Q_DECL_OVERRIDE;

signals:
    void enemyDestroyed();
    void enemyExecutionFinished();

};

#endif // ENEMY_HPP
