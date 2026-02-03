#include "MissileProjectile.h"
#include <iostream>
#include <cmath>

MissileProjectile::MissileProjectile(
    sf::Vector2f direction,
    float initialSpeed,
    float maxSpeed,
    int damage,
    const sf::Texture &texture)
    : Projectile(direction, initialSpeed, damage),
      maxSpeed(maxSpeed),
      acceleration(1000.f)
{
    sprite.emplace(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    sprite->setScale({0.7f, 0.7f});

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite->setRotation(sf::degrees(angle));
}

void MissileProjectile::update(float deltaTime)
{
    if (speed < maxSpeed)
        speed += acceleration * deltaTime;

    move(direction * speed * deltaTime);
}

void MissileProjectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!sprite)
        return;

    states.transform *= getTransform();
    target.draw(*sprite, states);
}

void MissileProjectile::destroy()
{
    Entity::destroy();
}
