#pragma once

#include "Projectile.h"
#include <SFML/Graphics.hpp>

class LaserProjectile : public Projectile
{
public:
    LaserProjectile(sf::Vector2f direction, float speed, int damage);
};
