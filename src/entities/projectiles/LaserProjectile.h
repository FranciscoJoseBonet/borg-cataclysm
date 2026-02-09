#pragma once
#include "Projectile.h"
#include <SFML/Graphics.hpp>

class LaserProjectile : public Projectile
{
public:
    // Constructor especifico para el Laser
    LaserProjectile(sf::Vector2f direction, float speed, int damage, const sf::Texture &texture, Faction faction);
};