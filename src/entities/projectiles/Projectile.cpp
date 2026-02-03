#include "Projectile.h"

Projectile::Projectile(sf::Vector2f direction, float speed, int damage)
    : Entity(1.f, "Projectile"),
      direction(direction),
      speed(speed),
      damage(damage)
{
}

void Projectile::update(float deltaTime)
{
    move(direction * speed * deltaTime);

    if (sprite)
        sprite->setPosition(getPosition());
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
        target.draw(*sprite, states);
}

void Projectile::destroy()
{
    Entity::destroy();
}