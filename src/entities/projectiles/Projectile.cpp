#include "Projectile.h"

Projectile::Projectile(sf::Vector2f direction, float speed, int damage, Faction faction)
    : Entity(1, "Projectile", faction),
      direction(direction),
      speed(speed),
      damage(damage)
{
}

void Projectile::update(float deltaTime)
{
    move(direction * speed * deltaTime);
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
    {
        states.transform *= getTransform();
        target.draw(*sprite, states);
    }
}

void Projectile::destroy()
{
    Entity::destroy();
}

sf::FloatRect Projectile::getBounds() const
{
    if (sprite)
    {
        return getTransform().transformRect(sprite->getGlobalBounds());
    }
    return sf::FloatRect();
}