#include "Weapon.h"
#include "../entities/projectiles/Projectile.h"

Weapon::Weapon(const std::string &name, float fireRate, float speed, int dmg)
    : name(name), projectileSpeed(speed), damage(dmg), currentCooldown(0.f)
{
    if (fireRate > 0)
        this->cooldownTime = 1.f / fireRate;
    else
        this->cooldownTime = 0.f;
}

void Weapon::update(float deltaTime)
{
    if (currentCooldown > 0.f)
        currentCooldown -= deltaTime;
}

Projectile *Weapon::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return nullptr;
    currentCooldown = cooldownTime;

    sf::Vector2f direction(0.f, -1.f);

    Projectile *p = new Projectile(direction, this->projectileSpeed, this->damage);
    p->setPosition(startPos);

    return p;
}