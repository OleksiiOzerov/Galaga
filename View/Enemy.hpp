#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "PixmapItem.hpp"

class Enemy : public PixmapItem
{
public:
    enum { Type = UserType + 1 };

    Enemy();

    virtual int type() const Q_DECL_OVERRIDE;

};

#endif // ENEMY_HPP
