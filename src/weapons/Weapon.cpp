#include "Weapon.h"
#include "../entities/projectiles/Projectile.h"
#include <memory>

Projectile *Weapon::Shoot(const sf::Vector2f &startPos)
{
    sf::Vector2f direction(0.f, -1.f);
    float speed = 500.f;
    int damage = 10;

    Projectile *p = new Projectile(direction, speed, damage);

    p->setPosition(startPos);

    return p;
}
