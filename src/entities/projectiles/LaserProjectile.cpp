#include "LaserProjectile.h"
#include <iostream>
#include <cmath>

LaserProjectile::LaserProjectile(
    sf::Vector2f direction,
    float speed,
    int damage,
    const sf::Texture &texture)
    : Projectile(direction, speed, damage)
{
    sprite.emplace(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    sprite->setScale({0.5f, 0.5f});

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite->setRotation(sf::degrees(angle));
}

void LaserProjectile::destroy()
{
    Entity::destroy();
}